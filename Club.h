#ifndef CLUB_H
#define CLUB_H

#include <map>
#include <QMutex>

#include "Room.h"
#include "types.h"


class Room;

class ClientSession;

class Club
{
public:
    Club();

    bool AddRoom(RoomId room_id);
    void AddToLobby(ClientSession *sess);

    bool RoomExists(RoomId room_id, bool lock = false);
    Room *GetRoom(RoomId room_id);
    std::map<RoomId, Room*> GetRooms();
    std::vector<ClientSession*> GetLobbyList();

    QMutex *GetMutex();

private:
    std::map<RoomId, Room*> rooms_;
    std::vector<ClientSession*> lobby_;
    QMutex *mutex_;
};

#endif // CLUB_H
