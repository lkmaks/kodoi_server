#include "Board.h"

Board::Board()
{
    mutex_ = new QMutex();
    epoch_id_ = 0;
    board_ = new AbstractBoard();
}

EpochId Board::GetEpochId() {
    return epoch_id_;
}

QMutex *Board::GetMutex() {
    return mutex_;
}

bool Board::ApplyAction(BoardAction action) {
    // assume epoch matches board epoch

    bool ok = false;

    if (action.type == BoardActionType::MOVE) {
        ok = board_->MakeMove(action.coords);
    }
    else if (action.type == BoardActionType::REDO) {
        ok = board_->Redo();
    }
    else if (action.type == BoardActionType::UNDO) {
        ok = board_->Undo();
    }
    else if (action.type == BoardActionType::UNDO_UNTIL) {
        ok = board_->UndoUntil(action.coords);
    }

    if (ok) {
        ++epoch_id_;
    }

    return ok;
}
