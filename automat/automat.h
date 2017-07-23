#ifndef AUTOMAT_H
#define AUTOMAT_H

#include "json-rpc/JsonRPCServer.h"

class AutomatController : public JsonRPCServer
{
public:
    AutomatController(Stream *stream);

    // TODO: Declare class methods and register JSON procedures.

    /*
    DECLARE_JSON_PROC(AutomatController, calibrate, bool);
    DECLARE_JSON_PROC(AutomatController, moveMouse, bool);

    BEGIN_JSON_REGISTRATION
        REGISTER_JSON_PROC(calibrate,      JSON_RPC_RET_TYPE_NUMERIC);
        REGISTER_JSON_PROC(moveMouse,      JSON_RPC_RET_TYPE_NUMERIC);
        REGISTER_JSON_PROC(pressKey,       JSON_RPC_RET_TYPE_NUMERIC);
        REGISTER_JSON_PROC(releaseKey,     JSON_RPC_RET_TYPE_NUMERIC);
        REGISTER_JSON_PROC(releaseAllKeys, JSON_RPC_RET_TYPE_NUMERIC);
        REGISTER_JSON_PROC(write,          JSON_RPC_RET_TYPE_NUMERIC);
        REGISTER_JSON_PROC(print,          JSON_RPC_RET_TYPE_NUMERIC);
        REGISTER_JSON_PROC(println,        JSON_RPC_RET_TYPE_NUMERIC);
    END_JSON_REGISTRATION
    */
};

#endif // AUTOMAT_H
