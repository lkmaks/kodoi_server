#include <QCoreApplication>

//#include "TCPServer.h"

#include "helpers.h"
#include "Message.h"

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);

//    TCPServer server(&a);

//    return a.exec();

    std::map<std::string, std::string> dc;
    std::map<std::string, std::string> dc2;
    dc["a"] = "bc";
    dc2["c"] = "d";
    dc2["d"] = "c";
    Protocol::Message m(dc);
    Protocol::Message m2(dc2);

    auto x = SerializeBinary<int>(10) + "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

    try {
        auto y = Protocol::take_new_messages(&x);
    } catch (std::exception e) {
        std::cerr << e.what() << std::endl;
    }

    std::cerr << 1;
}
