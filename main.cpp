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

    auto x = Protocol::serialize(m) + Protocol::serialize(m2) + QByteArray("aaa");
    auto y = Protocol::take_new_messages(&x);

    std::cerr << 1;
}
