#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>
#include <QAbstractSocket>

class TCPServer : public QObject
{
    Q_OBJECT
public:
    explicit TCPServer(QObject *parent = nullptr, int port = 12345);

    /// slots
    void onNewConnection();
    void onReadyRead();
    void onStateChanged(QAbstractSocket::SocketState);

    void Process(QByteArray &new_data);

private:
    int port_;
    QTcpServer *server_;
    QList<QTcpSocket*> sockets_;

    QByteArray data_;
    int data_pos_;
};

#endif // TCPSERVER_H
