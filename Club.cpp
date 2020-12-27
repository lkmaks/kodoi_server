#include <QMutexLocker>

#include "Club.h"


Club::Club()
{

}

bool Club::RoomExists(RoomId room_id) {
    QMutexLocker lock_guard(mutex_);
    return rooms_.count(room_id);
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


Room *Club::GetRoom(RoomId room_id) {
    QMutexLocker lock_guard(mutex_);
    return rooms_[room_id];
}
