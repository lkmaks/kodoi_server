#include <QCoreApplication>

#include "Message.h"
#include "cereal/archives/binary.hpp"

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);
//    return a.exec();
    QByteArray arr;
    Message mes(arr);
    mes.type = MessageType::MOVE;
    mes.room_id = 1000;
    mes.coords = {30000, 11111};

    std::ostringstream ss;
    cereal::BinaryOutputArchive oarchive(ss);
    oarchive(mes);

    std::istringstream ss2(ss.str());
    cereal::BinaryInputArchive iarchive(ss2);
    Message mes2(arr);
    iarchive(mes2);
    std::cout << ss.str() << std::endl;
    std::cout << ss.str().size() << std::endl;
}
