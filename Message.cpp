#include <QString>
#include <sstream>
#include <cereal/archives/binary.hpp>

#include "Message.h"

//int get_type_len(MessageType type) {
//    switch (type) {
//        case MessageType::CREATE:
//            return sizeof(CreateMessage);
//        case MessageType::ENTER:
//            return sizeof(EnterMessage);
//        case MessageType::ACTION:
//            return sizeof(BoardActionMessage);
//    }
//}

namespace Protocol {
    const int message_pref_len = sizeof(MessageSizeType);

    Message::Message(std::map<std::string, std::string> dict) : dict(dict) {}


    QByteArray serialize(const Message &mes) {
        QByteArray buf = Serialize<Message>(mes);
        QByteArray size_buf = SerializeBinary<MessageSizeType>(static_cast<MessageSizeType>(buf.size()));
        auto x = buf.size();
        return size_buf + buf;
    }


    std::vector<Message> take_new_messages(QByteArray *arr) {
        std::vector<Message> result;
        int ptr = 0;
        while (ptr + (int)sizeof(MessageSizeType) <= (int)arr->size()) {
            MessageSizeType map_size = DeserializeBinary<MessageSizeType>(arr->mid(ptr, message_pref_len));
            if (ptr + message_pref_len + map_size > arr->size()) {
                break;
            }

            Message new_msg = Deserialize<Message>(arr->mid(ptr + message_pref_len, map_size));
            result.push_back(new_msg);

            ptr += message_pref_len + map_size;
        }
        *arr = arr->right(arr->size() - ptr);
        return result;
    }
}
