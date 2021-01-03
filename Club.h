#ifndef CLUB_H
#define CLUB_H

#include <map>
#include <QMutex>

#include "Room.h"
#include "types.h"


class Room;

class Club
{
public:
    Club();

    bool RoomExists(RoomId room_id, bool lock = false);
    bool AddRoom(RoomId room_id);
    Room *GetRoom(RoomId room_id);

    QMutex *GetMutex();

private:
    std::map<RoomId, Room*> rooms_;
    QMutex *mutex_;
};

#endif // CLUB_H
