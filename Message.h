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


    const Key KEY_METHOD = "method";
    const Key KEY_STATUS = "status";

    const Value VALUE_STATUS_OK = "ok";
    const Value VALUE_STATUS_FAIL = "fail";

    const Value METHOD_CREATE = "create";
    const Value METHOD_ENTER = "enter";
    const Value METHOD_ACTION = "action";

    const Value METHOD_INIT = "init";
    const Value METHOD_UPDATE = "update";
    const Value METHOD_STATUS = "status";


    class Message {
        Message(std::map<std::string, std::string> dict = {});

        bool IsCorrect();

        bool has(Key key);
        Value operator[](Key key);


        // convinience constructors

        static Message Status(bool status);
        static Message Fail();
        static Message Ok();

        static Message Init(BoardAction action);
        static Message Update(BoardAction action);


        // convinience retrievers

        BoardAction GetAction();

        SERIALIZE(dict);
    private:
        std::map<Key, Value> dict;

    };

    QByteArray serialize(const Message &mes);
    std::vector<Message> take_new_messages(QByteArray *arr);
}



#endif // MESSAGE_H
