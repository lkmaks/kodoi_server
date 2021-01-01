#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <QMutex>

#include "AbstractBoard.h"
#include "helpers.h"


using EpochId = size_t;

enum class BoardActionType {
    MOVE,
    UNDO,
    REDO,
    UNDO_UNTIL
};

struct BoardAction {
    BoardActionType type;
    QPair<int, int> coords; // for move, undo_until
    EpochId epoch_id; // epoch to which this action is meant to apply
    SERIALIZE(type, coords.first, coords.second, epoch_id);
};

class Board
{
public:
    Board();
    EpochId GetEpochId();
    QMutex *GetMutex();
    bool ApplyAction(BoardAction action, bool lock = true);
    std::vector<BoardAction> GetInitSequence();
private:
    QMutex *mutex_;
    EpochId epoch_id_;
    AbstractBoard *board_;
};

#endif // BOARD_H
