#include <QMutexLocker>

#include "Club.h"


Club::Club()
{
    mutex_ = new QMutex();
}

bool Club::RoomExists(RoomId room_id, bool lock) {
    if (lock) {
        QMutexLocker lock_guard(mutex_);
        return rooms_.count(room_id);
    }
    else {
        return rooms_.count(room_id);
    }
}

bool Club::AddRoom(RoomId room_id) {
    QMutexLocker lock_guard(mutex_);
    if (RoomExists(room_id)) {
        return false;
    }
    else {
        rooms_[room_id] = new Room();
        return true;
    }
}

void Club::AddToLobby(ClientSession *sess) {
    lobby_.push_back(sess);
}


Room *Club::GetRoom(RoomId room_id) {
    QMutexLocker lock_guard(mutex_);
    return rooms_[room_id];
}

std::map<RoomId, Room*> Club::GetRooms() {
    return rooms_;
}

std::vector<ClientSession*> Club::GetLobbyList() {
    return lobby_;
}
