#include <Arduino.h>
#include <Keyboard.h>
#include <Mouse.h>

#include "automat.h"

AutomatController::AutomatController(Stream *stream) : JsonRPCServer(stream)
{
    // Intentionally empty. Nothing to do.
}

bool AutomatController::pressKey(aJsonObject *params)
{
    aJsonObject *_key = aJson.getObjectItem(params, "key");
    Keyboard.press(_key->valueint);
}

bool AutomatController::releaseKey(aJsonObject *params)
{
    aJsonObject *_key = aJson.getObjectItem(params, "key");
    Keyboard.release(_key->valueint);
}

bool AutomatController::releaseAllKeys(aJsonObject *params)
{
    Keyboard.releaseAll();
}

bool AutomatController::write(aJsonObject *params)
{
    aJsonObject *_key = aJson.getObjectItem(params, "key");
    Keyboard.write(_key->valueint);
}

bool AutomatController::print(aJsonObject *params)
{
    aJsonObject *_key = aJson.getObjectItem(params, "message");
    Keyboard.print(_key->valuestring);
}

bool AutomatController::println(aJsonObject *params)
{
    aJsonObject *_key = aJson.getObjectItem(params, "message");
    Keyboard.println(_key->valuestring);
}
