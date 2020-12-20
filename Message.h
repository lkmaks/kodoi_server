#ifndef MESSAGE_H
#define MESSAGE_H

#include<QByteArray>

const int MESSAGE_LEN = 10;

enum class MessageType {
    MOVE,
    UNDO,
    REDO,
    UNDO_UNTIL,
    INCORRECT
};

struct Message
{
    Message(const QByteArray &arr);
    MessageType mes_type;
    std::pair<int, int> coords;
};

#endif // MESSAGE_H
