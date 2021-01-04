#include <QMutexLocker>

#include "ClientSession.h"
#include "Message.h"
#include "helpers.h"


ClientSession::ClientSession(QTcpSocket *sock, Club *club) {
    club_ = club;

    room_id_ = -1;
    room_ = nullptr;

    sock_ = sock;
    data_ = new QByteArray();
}

ClientSession::~ClientSession() {
    delete data_;
}

void ClientSession::onReadyRead() {
    // new data available on sock_
    QByteArray new_data = sock_->readAll();
    data_->append(new_data);
    auto new_messages = take_new_messages(data_);
    for (auto mes : new_messages) {
        Process(mes);
    }
}

void ClientSession::Process(Message mes) {
    if (!mes.IsCorrect()) {
        return;
    }
    Value method = mes["type"];

    if (method == Protocol::METHOD_CREATE) {
        Value room_id;
        bool ok = club_->AddRoom(mes["room_id"]);
        Respond(Message::Status(ok));
        return;
    }
    else if (method == Protocol::METHOD_ENTER) {
        if (!club_->RoomExists(mes["room_id"])) {
            Respond(Message::Fail());
        }
        else {
            room_id_ = mes["room_id"];
            room_ = club_->GetRoom(room_id_);
            room_->AddSession(this);
            Respond(Message::Ok());
            Board *board = room_->GetBoard();
            for (auto action : board->GetInitSequence()) {
                Respond(Message::Init(action));
            }
        }
        return;
    }

    // now all left to process is action case
    if (method != Protocol::METHOD_ACTION) {
        return;
    }

    // have to be in a room to make actions
    if (!HasJoinedRoom()) {
        Respond(Message::Fail());
        return;
    }

    // current user is in a room
    Board *cur_board = room_->GetBoard();
    BoardAction cur_action = mes.GetAction();

    // need to check epoch and commit action atomically, otherwise two actions with same epoch may commit
    QMutexLocker locker(cur_board->GetMutex());

    // to apply action epoch should match
    if (cur_board->GetEpochId() != cur_action.epoch_id) {
        //Respond(Response::Fail()); -- for now
        return;
    }

    // epoch is right, try to apply action
    bool ok = cur_board->ApplyAction(cur_action, false);
    if (ok) {
        BroadcastToRoom(Message::Update(cur_action));
    }
}

bool ClientSession::HasJoinedRoom() {
    return room_ != nullptr;
}

RoomId ClientSession::GetRoomId() {
    return room_id_;
}

void ClientSession::Respond(Message resp) {
    sock_->write(Serialize<Message>(resp));
}

void ClientSession::BroadcastToRoom(Message resp) {
    for (auto sess : room_->GetSessions()) {
        sess->Respond(resp);
    }
}
