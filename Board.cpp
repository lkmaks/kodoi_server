#include "Board.h"
#include <QMutexLocker>
#include <iostream>

OnlineBoard::OnlineBoard()
{
    mutex_ = new QMutex();
    epoch_id_ = 0;
    board_ = new AbstractBoard();
}

EpochId OnlineBoard::GetEpochId() {
    return epoch_id_;
}

QMutex *OnlineBoard::GetMutex() {
    return mutex_;
}

bool OnlineBoard::ApplyAction(BoardAction action, bool lock) {
    if (lock) {
        QMutexLocker guard(mutex_);
        return ApplyActionNoLock(action);
    }
    else {
        return ApplyActionNoLock(action);
    }
}

std::vector<BoardAction> OnlineBoard::GetInitSequence() {
    QMutexLocker guard(mutex_);
    std::vector<BoardAction> res;
    for (auto move : board_->GetSequence(true)) {
        BoardAction action;
        action.type = BoardActionType::MOVE;
        action.coords = move;
        action.epoch_id = epoch_id_;
        res.push_back(action);
    }
    int all_seq_len = (int)res.size();
    for (int i = 0; i < all_seq_len - board_->MovesCount(); ++i) {
        BoardAction action;
        action.type = BoardActionType::UNDO;
        action.epoch_id = epoch_id_;
        res.push_back(action);
    }
    return res;
}


/// private

bool OnlineBoard::ApplyActionNoLock(BoardAction action) {
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

    std::cout << "Action applied: " <<
                 (int)action.type << " {" <<
                 action.coords.first << ", " << action.coords.second << "} " <<
                 action.epoch_id << std::endl;
    std::cout << "Result: " << ok << std::endl;
    std::cout << "Sequence now: ";
    for (auto move : board_->GetSequence()) {
        std::cout << "{" << move.first << ", " << move.second << "}" << " ";
    }
    std::cout << std::endl;

    return ok;
}
