#include <QMutexLocker>

#include "ClientSession.h"
#include "Message.h"
#include "helpers.h"


ClientSession::ClientSession(QTcpSocket *sock, Club *club) {
    club_ = club;

    room_id_ = -1;
    room_ = nullptr;

    sock_ = sock;
    data_ = {};
}

void ClientSession::onReadyRead() {
    // new data available on sock_
    *data_ += sock_->readAll();
    auto new_messages = take_new_messages(data_);
    for (auto mes : new_messages) {
        Process(mes);
    }
}

void ClientSession::Process(Message mes) {
    if (mes.type == MessageType::CREATE) {
        bool ok = club_->AddRoom(mes.room_id);
        Respond(Response(ok));
        return;
    }
    else if (mes.type == MessageType::ENTER) {
        if (!club_->RoomExists(mes.room_id)) {
            Respond(Response::Fail());
        }
        else {
            room_id_ = mes.room_id;
            room_ = club_->GetRoom(room_id_);
            Respond(Response::Ok());
        }
        return;
    }

    // now all left to process is action case
    if (mes.type != MessageType::ACTION) {
        return;
    }

    // have to be in a room to make actions
    if (!JoinedRoom()) {
        Respond(Response::Fail());
        return;
    }

    // current user is in a room
    Board *cur_board = room_->GetBoard();
    BoardAction cur_action = mes.action;

    // need to check epoch and commit action atomically, otherwise two actions with same epoch may commit
    QMutexLocker locker(cur_board->GetMutex());

    // to apply action epoch should match
    if (cur_board->GetEpochId() != cur_action.epoch_id) {
        Respond(Response::Fail());
        return;
    }

    // epoch is right, try to apply action
    bool ok = cur_board->ApplyAction(cur_action);
    if (ok) {
        BroadcastToRoom(Response::Update(cur_action));
    }
}

bool ClientSession::JoinedRoom() {
    return room_ != nullptr;
}

void ClientSession::Respond(Response resp) {
    std::string bytes_str = Serialize<Response>(resp);
    QByteArray bytes(bytes_str.c_str(), bytes_str.size());
    sock_->write(bytes);
}

void ClientSession::BroadcastToRoom(Response resp) {
    for (auto sess : room_->GetSessions()) {
        sess->Respond(resp);
    }
}
