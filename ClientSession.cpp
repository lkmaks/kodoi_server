#include <QMutexLocker>

#include "ClientSession.h"
#include "Message.h"
#include "helpers.h"


ClientSession::ClientSession(QTcpSocket *sock, Club *club, LoginSystem *login_system) {
    club_ = club;

    room_id_ = -1;
    room_ = nullptr;

    sock_ = sock;
    data_ = new QByteArray();

    login_system_ = login_system;
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

    if (method == Protocol::VALUE_METHOD_ROOMS_LIST) {
        for (auto pr : club_->GetRooms()) {
            Respond(Message::RoomAdded(pr.first));
        }
    }
    else if (method == Protocol::VALUE_METHOD_LOGIN) {
        auto name = mes[Protocol::KEY_LOGIN_NAME];
        auto password = mes[Protocol::KEY_LOGIN_PASSWORD];
        if (login_system_->TryLogin(name, password)) {
            name_ = name;
        }
    }
    else if (method == Protocol::VALUE_METHOD_CREATE) {
        Value room_id = mes[Protocol::KEY_ROOM_ID];
        bool ok = club_->AddRoom(room_id);
        if (ok) {
            for (auto sess : club_->GetLobbyList()) {
                RespondTo(Message::RoomAdded(room_id), sess);
            }
        }
        else {
            Respond(Message::Fail());
        }
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
            BroadcastToRoom(Message::UserEntered(name_));
        }
        return;
    }
    else if (method == Protocol::VALUE_METHOD_LEAVE) {
        if (room_ == nullptr) {
            Respond(Message::Fail());
        }

        room_->RemoveSession(this);
        Respond(Message::Ok());
        BroadcastToRoom(Message::UserLeft(name_));

        room_id_ = -1;
        room_ = nullptr;

    }
    else if (method == Protocol::VALUE_METHOD_NEED_INIT) {
        OnlineBoard *board = room_->GetBoard();
        for (auto action : board->GetInitSequence()) {
            Respond(Message::Init(action));
        }
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
    OnlineBoard *cur_board = room_->GetBoard();
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
    cur_action.epoch_id++;
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

void ClientSession::RespondTo(Message resp, ClientSession *sess) {
    sess->Respond(resp);
}

void ClientSession::BroadcastToRoom(Message resp) {
    for (auto sess : room_->GetSessions()) {
        sess->Respond(resp);
    }
}

void ClientSession::BroadcastToRoom(Message resp, RoomId room_id) {
    for (auto sess : club_->GetRoom(room_id)->GetSessions()) {
        sess->Respond(resp);
    }
}
