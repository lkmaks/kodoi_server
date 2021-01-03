#include <QCoreApplication>

#include "TCPServer.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TCPServer server(&a);

    std::cerr << Serialize<Response>(Response()).size() << std::endl;

    return a.exec();
}
