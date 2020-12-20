#include "TCPServer.h"

TCPServer::TCPServer(QObject *parent, int port) : QObject(parent), port_(port)
{
    server_ = new QTcpServer(this);
    connect(server_, &QTcpServer::newConnection, this, &TCPServer::onNewConnection);
    server_->listen(QHostAddress::Any, port_);
    data_ = QByteArray();
}

void TCPServer::onNewConnection() {
    QTcpSocket *clientSocket = server_->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::stateChanged, this, &TCPServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::stateChanged, this, &TCPServer::onStateChanged);
    sockets_.push_back(clientSocket);
}


void TCPServer::onReadyRead() {
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray new_data = sender->readAll();
    Process(new_data);
}

void TCPServer::onStateChanged(QAbstractSocket::SocketState state) {

}

void TCPServer::Process(QByteArray &new_data) {
    data_ += new_data;

}
