#ifndef CLIENTSESSION_H
#define CLIENTSESSION_H

#include <QTcpSocket>

#include "Club.h"
#include "Room.h"
#include "Message.h"
#include "Response.h"

class Club;
class Room;

class ClientSession
{
public:
    ClientSession(QTcpSocket *sock, Club *club);
    ~ClientSession();

    void onReadyRead();

    void Process(Message);
    void Respond(Response);
    void BroadcastToRoom(Response);

    RoomId GetRoomId();
    bool HasJoinedRoom();

private:

    Club *club_;

    int room_id_;
    Room *room_;

    QTcpSocket *sock_;
    QByteArray *data_;
};

#endif // CLIENTSESSION_H
