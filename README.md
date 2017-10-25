# <img width="300" alt="automat.ino" src="https://user-images.githubusercontent.com/26120940/28747795-7d87ae9c-745c-11e7-8e50-0f4f9044f699.png" />

**automat**.ino is a JSON-RPC API wrapper for Arduino's [Mouse and Keyboard libraries](https://www.arduino.cc/en/Reference/MouseKeyboard). It allows the mouse and keyboard functions to be invoked remotely from a PC over a serial port, effectively allowing a programmer to use an Arduino as a programmable hardware mouse and keyboard.

This library was written with the intention of being used to write video game bots that have lower probabilities of being detected by anti-cheat systems. Video games may use monitoring tools to inspect all processes running in a machine to detect the use of software macroing. Since the mouse and keyboard libraries allow an Arduino to appear as a native mouse or keyboard, this detection avenue is bypassed.

Only Arduino boards that have Mouse and Keyboard library support can be used. This includes the Leonardo, Micro, Due, and Zero boards as well as any other 32u4 based boards.

# Requirements

- A compatible Arduino board
- [Arduino IDE 1.6.x or later](https://www.arduino.cc/en/Main/Software)
- Microsoft Windows

# Dependencies

- [arduino-json-rpc](https://github.com/cloud-rocket/arduino-json-rpc) (included)
- [Boost C++ Libraries](http://www.boost.org/)

# Building

The **automat**.ino client code is built as a static library (`.lib`) that is to be linked into your C++ project.

1. Install Visual Studio 2017 (with the Visual C++ toolset and Windows 10 SDK modules).
2. Open the Visual Studio project file (`automat.vcxproj`) in Visual Studio 2017.
3. Configure `Project Properties` to use the correct Windows 10 SDK version.
4. Configure `Project Properties` to specify the Boost include and library paths.
5. Compile the static library: `Build` → `Build Solution`.

# Setup

1. Download the latest **automat**.ino release [here](https://github.com/nuttywhal/automat.ino/releases) and un-zip.
2. Connect the Arduino board to the computer via a USB connection.
3. In the Arduino IDE, add the *arduino-json-rpc* dependency to the Library Manager.
   - `Sketch` → `Include Library` → `Add .ZIP Library...` → `arduino/json-rpc.zip`.
4. Open `automat.ino` located in `arduino/` and upload the sketch (<kbd>CTRL + U</kbd>).

Once the Arduino sketch has been uploaded, the Arduino board is ready to respond to remote procedure call requests from the computer. **automat**.ino provides a C++ API that enables making those requests. To use it, add the `include/` and `lib/` directories to your project's include and linker library paths.

Make sure to disable mouse acceleration (A.K.A. pointer precision) under the Windows Mouse settings. Having mouse acceleration enabled may make the mouse functions in this API work incorrectly.

# Usage

Include the `Automat.h` header file in your project, then create an instance of the `automat::Automat` class by passing the name of the serial (COM) port that the Arduino is connected to as an argument to the constructor. The entire API is available through class methods.

```C++
#include <Automat.h>

/**
 * Moves the mouse cursor to the top-left of the screen,
 * waits 2 seconds, then moves the mouse cursor to the
 * bottom-right of the screen.
 */
void script(void)
{
    // Dimensions of the screen.
    const int WIDTH = 1920;
    const int HEIGHT = 1080;

    // Setup the Automat client.
    automat::Automat automat("COM4");
    automat.calibrate();

    // Write your script here.
    automat.wind_mouse(0, 0);
    automat.sleep(2000);
    automat.wind_mouse(WIDTH, HEIGHT);
}
```

# API

The API reference is available in the [GitHub Wiki](https://github.com/nuttywhal/automat.ino/wiki/API-Reference).

# License

**automat**.ino is licensed under the GNU General Public License (see `LICENSE.md` for details).
