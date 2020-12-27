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
    Board *GetBoard();
private:
    std::vector<ClientSession*> sessions_;
    Board *board_;
};

#endif // ROOM_H
