#ifndef AUTOMAT_H
#define AUTOMAT_H

#include "JsonRPCServer.h"

// The number of JSON-RPC procedures registered.
// This is used for initializing the server.
#define JSON_PROC_COUNT (6)

class AutomatController : public JsonRPCServer
{
public:
    AutomatController(Stream *stream);

    DECLARE_JSON_PROC(AutomatController, press,      bool);
    DECLARE_JSON_PROC(AutomatController, release,    bool);
    DECLARE_JSON_PROC(AutomatController, releaseAll, bool);
    DECLARE_JSON_PROC(AutomatController, write,      bool);
    DECLARE_JSON_PROC(AutomatController, print,      bool);
    DECLARE_JSON_PROC(AutomatController, println,    bool);

    BEGIN_JSON_REGISTRATION
        REGISTER_JSON_PROC(press,      JSON_RPC_RET_TYPE_NUMERIC);
        REGISTER_JSON_PROC(release,    JSON_RPC_RET_TYPE_NUMERIC);
        REGISTER_JSON_PROC(releaseAll, JSON_RPC_RET_TYPE_NUMERIC);
        REGISTER_JSON_PROC(write,      JSON_RPC_RET_TYPE_NUMERIC);
        REGISTER_JSON_PROC(print,      JSON_RPC_RET_TYPE_NUMERIC);
        REGISTER_JSON_PROC(println,    JSON_RPC_RET_TYPE_NUMERIC);
    END_JSON_REGISTRATION
};

#endif // AUTOMAT_H
