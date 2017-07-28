#include <Arduino.h>
#include <Keyboard.h>
#include <Mouse.h>

#include "automat.h"

AutomatController::AutomatController(Stream *stream) : JsonRPCServer(stream)
{
    // Intentionally empty. Nothing to do.
}

bool AutomatController::press(aJsonObject *params)
{
    aJsonObject *_key = aJson.getObjectItem(params, "key");
    Keyboard.press(_key->valueint);

    return true;
}

bool AutomatController::release(aJsonObject *params)
{
    aJsonObject *_key = aJson.getObjectItem(params, "key");
    Keyboard.release(_key->valueint);

    return true;
}

bool AutomatController::releaseAll(aJsonObject *params)
{
    Keyboard.releaseAll();

    return true;
}

bool AutomatController::write(aJsonObject *params)
{
    aJsonObject *_key = aJson.getObjectItem(params, "key");
    Keyboard.write(_key->valueint);

    return true;
}

bool AutomatController::print(aJsonObject *params)
{
    aJsonObject *_key = aJson.getObjectItem(params, "message");
    Keyboard.print(_key->valuestring);

    return true;
}

bool AutomatController::println(aJsonObject *params)
{
    aJsonObject *_key = aJson.getObjectItem(params, "message");
    Keyboard.println(_key->valuestring);

    return true;
}
