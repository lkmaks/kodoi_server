#ifndef ROOM_H
#define ROOM_H

#include <vector>

#include "Board.h"
#include "ClientSession.h"

class ClientSession;

class Room
{
public:
    Room();
    const std::vector<ClientSession*> &GetSessions();
    void AddSession(ClientSession *sess);
    void RemoveSession(ClientSession *sess);
    OnlineBoard *GetBoard();
private:
    QMutex *mutex_;
    std::vector<ClientSession*> sessions_;
    OnlineBoard *board_;
};

#endif // ROOM_H
