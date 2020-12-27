#include "Room.h"

Room::Room()
{

}

const std::vector<ClientSession*> &Room::GetSessions() {
    return sessions_;
}

Board *Room::GetBoard() {
    return board_;
}
