#include "TCPServer.h"

TCPServer::TCPServer(QObject *parent, int port) : QObject(parent), port_(port)
{
    club_ = new Club();
    server_ = new QTcpServer(this);
    connect(server_, &QTcpServer::newConnection, this, &TCPServer::onNewConnection);
    server_->listen(QHostAddress::Any, port_);
}

void TCPServer::onNewConnection() {
    std::cout << "new conn" << std::endl;
    QTcpSocket *clientSocket = server_->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &TCPServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::stateChanged, this, &TCPServer::onStateChanged);
    sessions_[clientSocket] = new ClientSession(clientSocket, club_);
}

void TCPServer::onReadyRead() {
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    ClientSession *cur_session = sessions_[sender];
    cur_session->onReadyRead();
}

void TCPServer::onStateChanged(QAbstractSocket::SocketState state) {
    if (state == QAbstractSocket::SocketState::UnconnectedState) {
        std::cerr << "on state changed to unconnected" << std::endl;
        QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
        sender->close();
        auto sess = sessions_[sender];
        if (sess->HasJoinedRoom()) {
            club_->GetRoom(sess->GetRoomId())->RemoveSession(sess);
        }
        sessions_[sender]->~ClientSession();
        sessions_.erase(sender);
    }
}
