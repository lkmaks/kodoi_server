#include "AbstractBoard.h"

AbstractBoard::AbstractBoard()
{
    board_size_ = 15;
    board_arr_.resize(board_size_);
    for (int i = 0; i < board_size_; ++i) {
        board_arr_[i].resize(board_size_);
    }
    cur_seq_len_ = 0;
}

bool AbstractBoard::MakeMove(QPair<int, int> cell) {
    if (!IsCell(cell) || board_arr_[cell.first][cell.second] != CellType::EMPTY) {
        return false;
    }

    if (cur_seq_len_ < sequence_.size() &&
            sequence_[cur_seq_len_] == cell) {
        ++cur_seq_len_;
    }
    else {
        while (sequence_.size() > cur_seq_len_) {
            sequence_.pop_back();
        }
        sequence_.push_back(cell);
        ++cur_seq_len_;
    }
    CellType new_cell = CellType::WHITE;
    if (cur_seq_len_ % 2 == 0) {
        new_cell = CellType::BLACK;
    }
    board_arr_[cell.first][cell.second] = new_cell;
    return true;
}

QPair<int, int> AbstractBoard::GetLastMove() {
    return sequence_[cur_seq_len_ - 1];
}

bool AbstractBoard::Undo() {
   if (cur_seq_len_ > 0) {
       auto cell = sequence_[cur_seq_len_ - 1];
       board_arr_[cell.first][cell.second] = CellType::EMPTY;
       --cur_seq_len_;
       return true;
   }
   return false;
}

bool AbstractBoard::UndoUntil(QPair<int, int> cell) {
    if (!IsCell(cell) || board_arr_[cell.first][cell.second] == CellType::EMPTY) {
        return false;
    }

    while (GetLastMove() != cell) {
        Undo();
    }

    return true;
}

bool AbstractBoard::Redo() {
    if (cur_seq_len_ < sequence_.size()) {
        auto cell = sequence_[cur_seq_len_];
        MakeMove(cell);
        return true;
    }
    return false;
}

bool AbstractBoard::Empty() {
    return cur_seq_len_ == 0;
}

CellType AbstractBoard::GetCell(QPair<int, int> cell) {
    return board_arr_[cell.first][cell.second];
}

StoneColor AbstractBoard::GetCurrentColor() {
    if (cur_seq_len_ % 2 == 0) {
        return StoneColor::BLACK;
    }
    else {
        return StoneColor::WHITE;
    }
}

int AbstractBoard::MovesCount() {
    return cur_seq_len_;
}

bool AbstractBoard::IsCell(QPair<int, int> cell) {
    int i = cell.first, j = cell.second;
    return 0 <= i && i < board_size_ && 0 <= j && j < board_size_;
}

bool AbstractBoard::IsRenjuLine(QPair<int, int> p1, QPair<int, int> p2) {
    int x = p1.first - p2.first;
    int y = p1.second - p2.second;
    return abs(x) == abs(y) || x * y == 0;
}
