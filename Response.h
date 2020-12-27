#ifndef RESPONSE_H
#define RESPONSE_H

#include <QByteArray>

#include "helpers.h"
#include "Board.h"

enum class ResponseType {
    STATUS, // to notify if action was successful
    UPDATE, // to send updates on actions commited by any user
    INIT // to send state in the beginning
};


struct Response
{
    ResponseType type;

    bool ok; // for status

    BoardAction action; // for update, init

    Response();
    Response(bool);

    static Response Ok();
    static Response Fail();
    static Response Update(BoardAction);

    SERIALIZE(type, ok, action);
};



#endif // RESPONSE_H
