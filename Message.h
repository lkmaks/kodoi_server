#ifndef MESSAGE_H
#define MESSAGE_H

#include <QByteArray>
#include <vector>

#include "helpers.h"
#include "Board.h"
#include "types.h"
#include "cereal/types/map.hpp"

#include <map>


//enum class MessageType {
//    CREATE,
//    ENTER,
//    ACTION
//};

//struct Message {
//    MessageType type;

//    Message(MessageType type);

//    SERIALIZE(type);
//};

//struct CreateMessage : Message {
//    RoomId room_id;

//    CreateMessage(RoomId room_id = 1);

//    SERIALIZE(type, room_id);
//};

//struct EnterMessage : Message {
//    RoomId room_id;

//    EnterMessage(RoomId room_id = 1);

//    SERIALIZE(type, room_id);
//};

//struct BoardActionMessage : Message {
//    BoardAction action;

//    BoardActionMessage(BoardAction action);

//    SERIALIZE(type, action);
//};


namespace Protocol {
    using MessageSizeType = int;
    using Key = std::string;
    using Value = std::string;

    struct Message {
        Message(std::map<std::string, std::string> dict = {});

        std::map<Key, Value> dict;

        SERIALIZE(dict);
    };

    QByteArray serialize(const Message &mes);
    std::vector<Message> take_new_messages(QByteArray *arr);
}



#endif // MESSAGE_H
