#ifndef AUTOMAT_H
#define AUTOMAT_H

#include <map>
#include <string>

#include <boost/asio.hpp>

namespace automat
{
	// The JSON-RPC server is listening at 115,200 baud.
	const int BAUD_RATE = 115200;

	// Represents a keyboard key button. The values are mapped to the
	// ASCII character set. Keyboard modifiers are defined in constants.h.
	typedef unsigned int Key;

	class Automat
	{
	public:
		/**
		 * Constructs an Automat object and initializes the I/O object
		 * to read from the specified serial device.
		 */
		Automat(std::string port);

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

		/**
		 * Construct a JSON-RPC request payload.
		 * \param Procedure - The name of the remote procedure to be invoked.
		 * \param Paramaters - A mapping of parameter names to paramater values.
		 */
		std::string construct_request(std::string, std::map<std::string, std::string>);

		/**
		 * Send a JSON-RPC request payload to the JSON-RPC server.
		 * \param Payload - JSON-RPC request payload.
		 */
		void send_request(std::string);

		/**
		 * Read the JSON-RPC response to a JSON-RPC request.
		 * \returns JSON-RPC response.
		 */
		std::string read_response(void);

		/**
		 * Parse a JSON-RPC response for the value to its "result" key.
		 * \returns The value of the "result" key.
		 */
		bool parse_response(std::string);
	};
}

#endif // AUTOMAT_H
