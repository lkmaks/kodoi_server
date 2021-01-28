#ifndef BOARDACTION_H
#define BOARDACTION_H

#include "inttypes.h"

using EpochId = uint64_t;

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
//    SERIALIZE(type, coords.first, coords.second, epoch_id);
};

#endif // BOARDACTION_H
