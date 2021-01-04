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
    using Key = QString;
    using Value = QString;


    const Key KEY_METHOD = "method";

    const Value METHOD_STATUS = "status";
    const Value METHOD_INIT = "init";
    const Value METHOD_UPDATE = "update";

    const Value METHOD_CREATE = "create";
    const Value METHOD_ENTER = "enter";
    const Value METHOD_ACTION = "action";


    const Key KEY_STATUS = "status";
    const Value VALUE_STATUS_OK = "ok";
    const Value VALUE_STATUS_FAIL = "fail";

    const Key KEY_ACTION_TYPE = "action_type";
    const Key KEY_ACTION_EPOCH_ID = "action_epoch";
    const Key KEY_ACTION_COORD_1 = "action_x";
    const Key KEY_ACTION_COORD_2 = "action_y";

    const Key KEY_ROOM_ID = "room_id";


    class Message {
    public:
        Message(std::map<Key, Value> dict_ = {});

        bool has(Key key);
        Value &operator[](Key key);


        /// convinience constructors

        // for server
        static Message Status(bool status);
        static Message Ok();
        static Message Fail();
        static Message Init(BoardAction action);
        static Message Update(BoardAction action);

        // for client
        static Message Create(RoomId room_id);
        static Message Enter(RoomId room_id);
        static Message Action(BoardAction);

        /// convinience retrievers

        BoardAction GetAction();

        bool IsCorrect();

        SERIALIZE(dict_);
    private:
        std::map<Key, Value> dict_;

        static Message ActionMessage(BoardAction action);

        bool ContainsCorrectAction();
    };

    QByteArray serialize(const Message &mes);
    std::vector<Message> take_new_messages(QByteArray *arr);
}



#endif // MESSAGE_H
