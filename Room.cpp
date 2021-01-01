#include "Room.h"
#include <QMutexLocker>

Room::Room()
{
    mutex_ = new QMutex();
    board_ = new Board();
}

const std::vector<ClientSession*> &Room::GetSessions() {
    QMutexLocker guard(mutex_);
    return sessions_;
}

void Room::AddSession(ClientSession *sess) {
    QMutexLocker guard(mutex_);
    sessions_.push_back(sess);
}

void Room::RemoveSession(ClientSession *sess) {
    QMutexLocker guard(mutex_);
    sessions_.erase(std::find(sessions_.begin(), sessions_.end(), sess));
}

Board *Room::GetBoard() {
    return board_;
}
