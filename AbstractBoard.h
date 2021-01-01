#ifndef ABSTRACTBOARD_H
#define ABSTRACTBOARD_H

#include <QVector>

enum class CellType {
    EMPTY,
    BLACK,
    WHITE
};

enum class StoneColor {
    BLACK,
    WHITE
};

class AbstractBoard
{
public:
    AbstractBoard();

    QPair<int, int> GetLastMove();
    bool Empty();
    CellType GetCell(QPair<int, int> cell);
    StoneColor GetCurrentColor();
    int MovesCount();
    QVector<QPair<int, int> > GetSequence(bool all = false);

    bool MakeMove(QPair<int, int> cell);
    bool Undo();
    bool UndoUntil(QPair<int, int> cell);
    bool Redo();

    static bool IsRenjuLine(QPair<int, int> p1, QPair<int, int> p2);
    bool IsCell(QPair<int, int> cell);
private:
    int board_size_;
    QVector<QVector<CellType> > board_arr_;
    QVector<QPair<int, int> > sequence_; // position up to "going back" point
    int cur_seq_len_; // prefix of sequence currently on the board
};

#endif // ABSTRACTBOARD_H
