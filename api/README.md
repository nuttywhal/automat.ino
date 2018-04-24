# API Reference

**Note**: All methods marked with ​:cyclone:​ provide additional anti-cheat system detection prevention by either humanizing mouse movements and keyboard inputs and/or adding random delays.

## Mouse Methods

[#]() `Automat::calibrate()`

Invoke this function to calibrate your mouse sensitivity settings with the Arduino board to determine a calibration factor used to map the units used internally by Arduino's [`Mouse.move()`](https://www.arduino.cc/en/Reference/MouseMove) function to pixel units. This function is what enables this API to achieve absolute positioning. **Invoke this function before invoking any other mouse functions.**

[#]() `Automat::move(x, y)`

Snaps the mouse cursor to the specified location.

- **x**: x-coordinate of destination location - `unsigned int`
- **y**: y-coordinate of destination location - `unsigned int`

[#]() `Automat::linear_move(x, y)`

Moves the mouse cursor to the specified location along a linear path.

- **x**: x-coordinate of destination location - `unsigned int`
- **y**: y-coordinate of destination location - `unsigned int`

[#]() `Automat::wind_move(x, y, gravity, wind, minWait, maxWait, maxStep, targetArea)` :cyclone:

Moves the mouse cursor to the specified location along randomly-generated spline defined by the `gravity` and `wind` parameters. This algorithm was designed by BenLand100 & Flight and ported from the [SRL Resource Library](https://github.com/SRL/SRL).

- **x**: x-coordinate of destination location - `unsigned int`
- **y**: y-coordinate of destination location - `unsigned int`
- **gravity** = `2.0`: A factor that keeps the mouse cursor along a linear path to the destination - `double`
- **wind** = `2.0`: A factor that causes the mouse cursor to waver away from the path to the destination - `double`
- **minWait** = `2.0`: Minimum time (in milliseconds) to wait before each step - `double`
- **maxWait** = `10.0`: Maximum time (in milliseconds) to wait before each step - `double`
- **maxStep** = `20.0`: Maximum distance (in pixels) to move in each step - `double`
- **targetArea** = `50.0`: Radius of destination location to reach before homing into destination - `double`

[#]() `Automat::click(button)` :cyclone:

Sends a momentary click to the computer, holding the mouse button down for a random interval between 10-50ms before releasing.

- **button** = `automat::M_LEFT`: Mouse button to click - `Button`

[#]() `Automat::double_click(button)` :cyclone:

Sends a double-click action to the computer—equivalent to calling `Automat::click(button)` twice.

- **button** = `automat::M_LEFT`: Mouse button to click - `Button`

[#]() `Automat::hold(button)`

Sends a button press to the computer. This will click and continuously hold the mouse button. A press is cancelled with `Automat::unhold(button)`.

- **button** = `automat::M_LEFT`: Mouse button to click - `Button`


[#]() `Automat::unhold(button)`

Sends a button release message to the computer. This will release a button press made by `Automat::hold(button)`.

- **button** = `automat::M_LEFT`: Mouse button to click - `Button`

## Keyboard Methods

[#]() `Automat::write(key)`

Sends a keystroke to the computer.

**key**: Keyboard button to press - `Key`

[#]() `Automat::tap(key)` :cyclone:

Sends a keystroke to the computer, holding the key down for a random interval between 10-50ms before releasing.

- **key**: Keyboard button to press - `Key`

[#]() `Automat::press(key)`

Press and hold a key as if it were pressed and held on your keyboard. This is useful when using modifier keys. A key is released with `Automat::release(key)` or `Automat::releaseAll()`.

- **key**: Keyboard button to press - `Key`

[#]() `Automat::release(key)`

Releases the specified key held down by `Automat::press(key)`.

- **key**: Keyboard button to press - `Key`

[#]() `Automat::releaseAll()`

Releases all keys held down by `Automat::press(key)`.

[#]() `Automat::print(message)`

Types a message string. The string is sent as a single keystroke.

- **message**: A message to type - `std::string`

[#]() `Automat::println(message)`

Types a message string followed by a newline character and a carriage return. The string is sent as a single keystroke.

- **message**: A message to type - `std::string`

[#]() `Automat::type(message)` :cyclone:

Types a message like a human. This function attempts to emulate human typing by occasionally making mistakes and amending them with the backspace key.

- **message**: A message to type - `std::string`
- **wpm** = `100`: Rate (words per minute) to type the message (assuming five characters per word) - `unsigned int`
- **mistakes** = `true`: If true, typing mistakes will be made by the library and amended - `bool`
- **accuracy** = `98`: Probability of making a mistake when typing each character in the message - `unsigned int`


## Constants

| Name                | Identifier          | Value  |
| ------------------- | ------------------- | ------ |
| Left Mouse Button   | `automat::M_LEFT`   | `0x01` |
| Right Mouse Button  | `automat::M_RIGHT`  | `0x02` |
| Middle Mouse Button | `automat::M_MIDDLE` | `0x04` |
| All Mouse Buttons   | `automat::M_ALL`    | `0x07` |
| Left Control        | `automat::L_CTRL`   | `0x80` |
| Left Shift          | `automat::L_SHIFT`  | `0x81` |
| Left Alt            | `automat::L_ALT`    | `0x82` |
| Left GUI            | `automat::L_GUI`    | `0x83` |
| Right Control       | `automat::R_CTRL`   | `0x84` |
| Right Shift         | `automat::R_SHIFT`  | `0x85` |
| Right Alt           | `automat::R_ALT`    | `0x86` |
| Right GUI           | `automat::R_GUI`    | `0x87` |
| Up Arrow Key        | `automat::UP`       | `0xDA` |
| Down Arrow Key      | `automat::DOWN`     | `0xD9` |
| Left Arrow Key      | `automat::LEFT`     | `0xD8` |
| Right Arrow Key     | `automat::RIGHT`    | `0xD7` |
| Backspace           | `automat::BACK`     | `0xB2` |
| Tab                 | `automat::TAB`      | `0xB3` |
| Return              | `automat::RET`      | `0xB0` |
| Escape              | `automat::ESC`      | `0xB1` |
| Insert              | `automat::INS`      | `0xD1` |
| Delete              | `automat::DEL`      | `0xD4` |
| Page Up             | `automat::PG_UP`    | `0xD3` |
| Page Down           | `automat::PG_DN`    | `0xD6` |
| Home                | `automat::HOME`     | `0xD2` |
| End                 | `automat::END`      | `0xD5` |
| Caps Lock           | `automat::CAPS`     | `0xC1` |
| F1                  | `automat::F1`       | `0xC2` |
| F2                  | `automat::F2`       | `0xC3` |
| F3                  | `automat::F3`       | `0xC4` |
| F4                  | `automat::F4`       | `0xC5` |
| F5                  | `automat::F5`       | `0xC6` |
| F6                  | `automat::F6`       | `0xC7` |
| F7                  | `automat::F7`       | `0xC8` |
| F8                  | `automat::F8`       | `0xC9` |
| F9                  | `automat::F9`       | `0xCA` |
| F10                 | `automat::F10`      | `0xCB` |
| F11                 | `automat::F11`      | `0xCC` |
| F12                 | `automat::F12`      | `0xCD` |