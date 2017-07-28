#include <Arduino.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <limits.h>

#include "automat.h"

AutomatController::AutomatController(Stream *stream)
    : JsonRPCServer(stream), _calibrationFactor(1.f)
{
    // Intentionally empty. Nothing to do.
}

bool AutomatController::calibrate(aJsonObject *params)
{
    aJsonObject *_x = aJson.getObjectItem(params, "x");
    aJsonObject *_y = aJson.getObjectItem(params, "y");
    aJsonObject *_f = aJson.getObjectItem(params, "factor");

    int   x = _x->valueint;
    int   y = _y->valueint;
    float f = _f->valuefloat;

    moveMouseRelative(x, y, f);
    _calibrationFactor = f;

    return true;
}

bool AutomatController::moveMouse(aJsonObject *params)
{
    aJsonObject *_a_x = aJson.getObjectItem(params, "a_x");
    aJsonObject *_a_y = aJson.getObjectItem(params, "a_y");
    aJsonObject *_b_x = aJson.getObjectItem(params, "b_x");
    aJsonObject *_b_y = aJson.getObjectItem(params, "b_y");

    int a_x = _a_x->valueint;
    int a_y = _a_y->valueint;
    int b_x = _b_x->valueint;
    int b_y = _b_y->valueint;

    moveMouseRelative(b_x - a_x, b_y - a_y, _calibrationFactor);

    return true;
}

bool AutomatController::click(aJsonObject *params)
{
    aJsonObject *_key = aJson.getObjectItem(params, "button");
    Mouse.click(_key->valueint);

    return true;
}

bool AutomatController::hold(aJsonObject *params)
{
    aJsonObject *_key = aJson.getObjectItem(params, "button");
    Mouse.press(_key->valueint);

    return true;
}

bool AutomatController::unhold(aJsonObject *params)
{
    aJsonObject *_key = aJson.getObjectItem(params, "button");
    Mouse.release(_key->valueint);

    return true;
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

void AutomatController::moveMouseRelative(int x, int y, float factor)
{
    /*
     * Mouse.move() takes signed char types as arguments for the x and y
     * movements. So if we want to move the mouse more than -127 and 127
     * units, then we need to chain multiple calls together.
     */

    int x_normalized = x * factor;
    int y_normalized = y * factor;

    /*
     * It seems that passing a value of SCHAR_MIN to Mouse.move() will
     * actually result in an underflow to 0 and not move the mouse at
     * all. We will use -SCHAR_MAX instead of SCHAR_MIN for that reason.
     */

    while (x_normalized != 0 || y_normalized != 0) {

        if (x_normalized > SCHAR_MAX) {
            Mouse.move(SCHAR_MAX, 0);
            x_normalized -= SCHAR_MAX;
        }

        else if (x_normalized < -SCHAR_MAX) {
            Mouse.move(-SCHAR_MAX, 0);
            x_normalized += SCHAR_MAX;
        }

        else if (y_normalized > SCHAR_MAX) {
            Mouse.move(0, SCHAR_MAX);
            y_normalized -= SCHAR_MAX;
        }

        else if (y_normalized < -SCHAR_MAX) {
            Mouse.move(0, -SCHAR_MAX);
            y_normalized += SCHAR_MAX;
        }

        else {
            Mouse.move(x_normalized, y_normalized);
            x_normalized = 0;
            y_normalized = 0;
        }

    }
}
