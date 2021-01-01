#include "Response.h"

Response::Response() {}

Response::Response(bool ok) : type(ResponseType::STATUS), ok(ok) {}

Response Response::Ok() {
    return Response(true);
}

Response Response::Fail() {
    return Response(false);
}

Response Response::Init(BoardAction action) {
    Response res;
    res.type = ResponseType::INIT;
    res.action = action;
    return res;
}

Response Response::Update(BoardAction action) {
    Response res;
    res.type = ResponseType::UPDATE;
    res.action = action;
    return res;
}

