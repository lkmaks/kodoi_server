#include <QCoreApplication>

#include "TCPServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TCPServer server(&a);

    return a.exec();

//    std::map<std::string, std::string> dc;
//    std::map<std::string, std::string> dc2;
//    dc["a"] = "bc";
//    dc2["c"] = "d";
//    dc2["d"] = "c";
//    Protocol::Message m = Protocol::Message::Enter("abc");
//    BoardAction ac;
//    ac.coords.first = 2;
//    Protocol::Message m2 = Protocol::Message::Action(ac);

//    auto x = Protocol::SerializeMessage(m);
//    auto xx = Protocol::SerializeMessage(m2);

//    auto buf = (x + xx);

//    try {
//        auto y = Protocol::take_new_messages(&buf);
//        auto zz = y[1].GetAction();
//        std::cerr << 1;
//    } catch (std::exception e) {
//        std::cerr << e.what() << std::endl;
//    }

}
