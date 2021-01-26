#ifndef CLIENTSESSION_H
#define CLIENTSESSION_H

#include <QTcpSocket>

#include "Club.h"
#include "Room.h"
#include "Message.h"
#include "LoginSystem.h"

class Club;
class Room;

using Protocol::Message;
using Protocol::take_new_messages;
using Protocol::Key;
using Protocol::Value;

class ClientSession
{
public:
    ClientSession(QTcpSocket *sock, Club *club);
    ~ClientSession();

    void onReadyRead();

    void Process(Message);
    void Respond(Message);
    void BroadcastToRoom(Message);

    RoomId GetRoomId();
    bool HasJoinedRoom();

private:
    // essence
    Club *club_;
    QTcpSocket *sock_;
    QByteArray *data_;
    LoginSystem *login_system_;

    // context
    RoomId room_id_;
    Room *room_;

    QString name_;

};

#endif // CLIENTSESSION_H
