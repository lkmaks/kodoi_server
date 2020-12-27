#include "TCPServer.h"

TCPServer::TCPServer(QObject *parent, int port) : QObject(parent), port_(port)
{
    club_ = new Club();
    server_ = new QTcpServer(this);
    connect(server_, &QTcpServer::newConnection, this, &TCPServer::onNewConnection);
    server_->listen(QHostAddress::Any, port_);
}

void TCPServer::onNewConnection() {
    QTcpSocket *clientSocket = server_->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::stateChanged, this, &TCPServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::stateChanged, this, &TCPServer::onStateChanged);
    sessions_[clientSocket] = new ClientSession(clientSocket, club_);
}

void TCPServer::onReadyRead() {
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    ClientSession *cur_session = sessions_[sender];
    cur_session->onReadyRead();
}

void TCPServer::onStateChanged(QAbstractSocket::SocketState state) {
    // handle disconnection
}
