#ifndef AUTOMAT_H
#define AUTOMAT_H

#include <map>
#include <set>
#include <string>

#include <boost/asio.hpp>
#include <windows.h>

#include "constants.h"

namespace automat
{
	// Represents a keyboard key or a mouse button. The values are mapped to
	// the ASCII character set. Keyboard modifiers and mouse button constants
	// are defined in constants.h.
	typedef unsigned int Key;

	// Stores screen coordinates of the cursor.
	//   .x : the x-coordinate of the point.
	//   .y : the y-coordinate of the point.
	typedef POINT point;

	class Automat
	{
	public:
		/**
		 * Constructs an Automat object and initializes the I/O object
		 * to read from the specified serial device.
		 */
		Automat(std::string port);

		/**
		 * The Arduino mouse library has no way of knowing how to move the
		 * mouse cursor to a particular coordinate location on the screen.
		 * It can only move some units (which is not necessarily in pixels)
		 * relative to the cursor's current location.
		 *
		 * In order to achieve absolute positioning, there needs to be some
		 * way to find a factor that, when multiplied by the units that the Arduino
		 * uses to move the mouse, yields a pixel.
		 *
		 * With this calibration factor, we can instruct the Arduino to move
		 * `n' pixels (or factor * Arduino units) relative to the current mouse
		 * position.
		 */
		void calibrate();

		/**
		 * Move the mouse cursor from the current location to the specified location.
		 *
		 * \param X - The x-coordinate of the destination location.
		 * \param Y - The y-coordinate of the destination location.
		 */
		bool move(unsigned int, unsigned int);

		/**
		 * Move the mouse cursor from the current location to the specified location
		 * along a linear path.
		 *
		 * \param X - The x-coordinate of the destination location.
		 * \param Y - The y-coordinate of the destination location.
		 */
		bool linear_move(unsigned int, unsigned int);

		/**
		 * Sends a momentary click to the computer at the location of the cursor.
		 * This is the same as pressing and immediately releasing the mouse button.
		 *
		 * \param Button - The mouse button to press with.
		 */
		bool click(Key key = M_LEFT);

		/**
		 * Sends a button press to a connected computer. A press is the equivalent of
		 * clicking and continuously holding the mouse button. A press is cancelled
		 * with unhold().
		 *
		 * \param Button - The mouse button to hold down.
		 */
		bool hold(Key key = M_LEFT);

		/**
		 * Sends a message that a previously pressed button (invoked through hold()) is
		 * released. Defaults to the left button.
		 *
		 * \param Button - The mouse button to release.
		 */
		bool unhold(Key key = M_LEFT);

		/**
		 * When called, press() functions as if a key were pressed and held
		 * on your keyboard. Useful when using modifier keys. To end the key
		 * press, use release() or releaseAll().
		 */
		bool press(Key);

		/**
		 * Lets go of the specified key. See press().
		 */
		bool release(Key);

		/**
		 * Lets go of all keys currently pressed. See press().
		 */
		bool release_all(void);

		/**
		 * Sends a keystroke to a connected computer. This is similar to
		 * pressing and releasing a key on your keyboard. You can send some
		 * ASCII characters or the additional keyboard modifiers and special
		 * keys.
		 */
		bool write(Key);

		/**
		 * Sends a keystroke to a connected computer. The string will be sent
		 * as a single keystroke.
		 */
		bool print(std::string);

		/**
	 	 * Sends a keystroke to a connected computer, followed by a newline
	 	 * and a carriage return. The string will be sent as a single
	 	 * keystroke.
		 */
		bool println(std::string);

		/**
		 * Halt the current thread for the specified number of milliseconds.
		 */
		void sleep(unsigned int);

	private:
		boost::asio::io_service _io;
		boost::asio::serial_port _serial;

		// The baud rate that the JSON-RPC server is listening at.
		const int BAUD_RATE = 115200;

		// A list of parameter names that have non-string values.
		// See documentation for construct_request.
		const std::set<std::string> _nonStringParameters = {
			"key",
			"button",
			"factor",
			"a_x",
			"a_y",
			"b_x",
			"b_y",
			"x",
			"y"
		};

		/**
		 * Construct a JSON-RPC request payload.
		 *
		 * \param Procedure - The name of the remote procedure to be invoked.
		 * \param Paramaters - A mapping of parameter names to paramater values.
		 */
		std::string construct_request(std::string, std::map<std::string, std::string>);

		/**
		 * Send a JSON-RPC request payload to the JSON-RPC server.
		 *
		 * \param Payload - JSON-RPC request payload.
		 */
		void send_request(std::string);

		/**
		 * Read the JSON-RPC response to a JSON-RPC request.
		 *
		 * \returns JSON-RPC response.
		 */
		std::string read_response(void);

		/**
		 * Parse a JSON-RPC response for the value to its "result" key.
		 *
		 * \returns The value of the "result" key.
		 */
		bool parse_response(std::string);

		/**
		 * Retrieves the position of the mouse cursor, in screen coordinates.
		 *
		 * \returns The cursor location.
		 */
		point get_cursor_pos(void);

		/**
		 * Returns the coordinates of the bottom right corner of the screen.
		 *
		 * \returns The dimensions of the screen.
		 */
		point get_resolution(void);
	};
}

#endif // AUTOMAT_H
