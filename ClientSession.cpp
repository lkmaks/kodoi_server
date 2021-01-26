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

    name_ = login_system_->MakeGuestName();
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
    Value method = mes[Protocol::KEY_METHOD];

    if (method == Protocol::VALUE_METHOD_LOGIN) {
        auto name = mes[Protocol::KEY_LOGIN_NAME];
        auto password = mes[Protocol::KEY_LOGIN_PASSWORD];
        if (login_system_->TryLogin(name, password)) {
            name_ = name;
        }
    }
    if (method == Protocol::VALUE_METHOD_CREATE) {
        Value room_id;
        bool ok = club_->AddRoom(mes[Protocol::KEY_ROOM_ID]);
        Respond(Message::Status(ok));
        return;
    }
    else if (method == Protocol::VALUE_METHOD_ENTER) {
        room_id_ = mes[Protocol::KEY_ROOM_ID];
        if (!club_->RoomExists(room_id_)) {
            Respond(Message::Fail());
        }
        else {
            room_ = club_->GetRoom(room_id_);
            room_->AddSession(this);
            Respond(Message::Ok());
            Board *board = room_->GetBoard();
            for (auto action : board->GetInitSequence()) {
                Respond(Message::Init(action));
            }
            BroadcastToRoom(Message::UserEntered(name_));
        }
        return;
    }
    else if (method == Protocol::VALUE_METHOD_LEAVE) {
        room_->RemoveSession(this);
        room_id_ = -1;
        room_ = nullptr;
    }

    // now all left to process is action case
    if (method != Protocol::VALUE_METHOD_ACTION) {
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
    sock_->write(SerializeMessage(resp));
}

void ClientSession::BroadcastToRoom(Message resp) {
    for (auto sess : room_->GetSessions()) {
        sess->Respond(resp);
    }
}
