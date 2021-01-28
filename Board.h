#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <QMutex>

#include "AbstractBoard.h"
#include "BoardAction.h"

class OnlineBoard
{
public:
    OnlineBoard();
    EpochId GetEpochId();
    QMutex *GetMutex();
    bool ApplyAction(BoardAction action, bool lock = true);
    std::vector<BoardAction> GetInitSequence();
private:
    QMutex *mutex_;
    EpochId epoch_id_;
    AbstractBoard *board_;

    bool ApplyActionNoLock(BoardAction action);
};

#endif // BOARD_H
