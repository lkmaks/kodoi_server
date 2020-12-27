#ifndef CLUB_H
#define CLUB_H

#include <map>
#include <QMutex>

#include "Room.h"


using RoomId = unsigned int;

class Club
{
public:
    Club();

    bool RoomExists(RoomId room_id);
    bool AddRoom(RoomId room_id);
    Room *GetRoom(RoomId room_id);

    QMutex *GetMutex();

private:
    std::map<RoomId, Room*> rooms_;
    QMutex *mutex_;
};

#endif // CLUB_H
