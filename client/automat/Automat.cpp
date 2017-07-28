#include <chrono>
#include <cstdlib>
#include <sstream>
#include <thread>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "automat.h"

namespace automat
{
	Automat::Automat(std::string port)
		: _io(), _serial(_io, port)
	{
		// Set the serial communication speed.
		_serial.set_option(boost::asio::serial_port_base::baud_rate(BAUD_RATE));
	}

	void Automat::calibrate()
	{
		double factor = 1.0;
		point resolution = get_resolution();

		for (int step = 50; step < (int)(resolution.y * 0.4); step++) {

			// Dummy values for the first conditional test.
			point a = get_cursor_pos();
			point b = { a.x, a.y };

			while (std::abs(std::abs(b.x - a.x) - step) > 1 &&
				   std::abs(std::abs(b.y - a.y) - step) > 1 &&
				   std::abs(b.x - a.x) < resolution.x / 2.0 &&
				   std::abs(b.y - a.y) < resolution.y / 2.0) {

				// Keep mouse movements toward the center of the screen.
				a = get_cursor_pos();
				int x_dir = (a.x < resolution.x / 2.0) ? 1 : -1;
				int y_dir = (a.y < resolution.y / 2.0) ? 1 : -1;

				// Send JSON-RPC request to invoke the calibrate method
				// on the Arduino. This will move the mouse (x * factor,
				// y * factor) units relative to the current mouse position.
				const std::string proc_name("calibrate");
				std::map<std::string, std::string> params;

				params["x"] = std::to_string(x_dir * step);
				params["y"] = std::to_string(y_dir * step);
				params["factor"] = std::to_string(factor);

				send_request(construct_request(proc_name, params));
				parse_response(read_response());

				// Make adjustments to the calibration factor.
				b = get_cursor_pos();
				factor += (std::abs(b.x - a.x) < step ? 0.001 : -0.001);
			}
		}
	}

	bool Automat::move(int x, int y)
	{
		point curr = get_cursor_pos();
		point dest = { x, y };

		const std::string proc_name("moveMouse");
		std::map<std::string, std::string> params;

		params["a_x"] = std::to_string(curr.x);
		params["a_y"] = std::to_string(curr.y);
		params["b_x"] = std::to_string(dest.x);
		params["b_y"] = std::to_string(dest.y);

		// Construct and send a remote procedure call request.
		send_request(construct_request(proc_name, params));

		// Receive and parse the response.
		return parse_response(read_response());
	}

	bool Automat::press(Key key)
	{
		const std::string proc_name("press");
		std::map<std::string, std::string> params;
		params["key"] = std::to_string(key);

		// Construct and send a remote procedure call request.
		send_request(construct_request(proc_name, params));

		// Receive and parse the response.
		return parse_response(read_response());
	}

	bool Automat::release(Key key)
	{
		const std::string proc_name("release");
		std::map<std::string, std::string> params;
		params["key"] = std::to_string(key);

		// Construct and send a remote procedure call request.
		send_request(construct_request(proc_name, params));

		// Receive and parse the response.
		return parse_response(read_response());
	}

	bool Automat::release_all()
	{
		const std::string proc_name("releaseAll");
		std::map<std::string, std::string> params;

		// Construct and send a remote procedure call request.
		send_request(construct_request(proc_name, params));

		// Receive and parse the response.
		return parse_response(read_response());
	}

	bool Automat::write(Key key)
	{
		const std::string proc_name("write");
		std::map<std::string, std::string> params;
		params["key"] = std::to_string(key);

		// Construct and send a remote procedure call request.
		send_request(construct_request(proc_name, params));

		// Receive and parse the response.
		return parse_response(read_response());
	}

	bool Automat::print(std::string message)
	{
		const std::string proc_name("print");
		std::map<std::string, std::string> params;
		params["message"] = message;

		// Construct and send a remote procedure call request.
		send_request(construct_request(proc_name, params));

		// Receive and parse the response.
		return parse_response(read_response());
	}

	bool Automat::println(std::string message)
	{
		/*
			const std::string proc_name("println");
			std::map<std::string, std::string> params;
			params["message"] = message;

			// Construct and send a remote procedure call request.
			send_request(construct_request(proc_name, params));

			// Receive and parse the response.
			return parse_response(read_response());
		*/

		// For whatever reason, the println method won't work.
		// It doesn't append a newline at the end of the message
		// so we will manually print the newline.

		print(message);
		return write(0xB0);
	}

	void Automat::sleep(unsigned int ms)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}

	std::string Automat::construct_request(std::string proc_name, std::map<std::string, std::string> arg_map)
	{
		// Non-string parameter values.
		std::vector<std::string> values;

		// Use property tree structure to store key/value pairs.
		boost::property_tree::ptree pt;
		boost::property_tree::ptree params;

		for (auto const &pair : arg_map) {
			// Store parameter names and values.
			params.put(pair.first, pair.second);

			// Store non-string parameter values for the hack described later.
			if (nonStringParameters.find(pair.first) != nonStringParameters.end()) {
				values.push_back(pair.second);
			}
		}

		pt.put("method", proc_name);
		pt.add_child("params", params);

		// Serialize the property tree into JSON.
		std::stringstream stream;
		write_json(stream, pt);
		std::string json = stream.str();

		// The thing about using a property tree structure to construct JSON is that
		// after it is serialized, all type information is lost. All values become
		// enclosed in double quotes, marking them as strings. This little hack will
		// replace the string representation of the int value of the key, "key", with
		// the actual int value.
		
		for (auto const &value : values) {
			boost::algorithm::replace_all(json, "\"" + value + "\"", value);
		}
		
		return json;
	}

	std::string Automat::read_response(void)
	{
		std::string response;
		char next = NULL;

		// Read character by character from the serial buffer until the '}'
		// is encountered, delimiting the end of the JSON response. This
		// assumes that there will be no nested JSON objects in the response.

		while (next != '}') {
			boost::asio::read(_serial, boost::asio::buffer(&next, 1));
			response += next;
		}

		return response;
	}

	void Automat::send_request(std::string request)
	{
		// Send the JSON request payload to the JSON-RPC server.
		auto payload = boost::asio::buffer(request.c_str(), request.size());
		boost::asio::write(_serial, payload);
	}

	bool Automat::parse_response(std::string response)
	{
		// Read response into string stream.
		std::stringstream stream;
		stream << response;

		// Pass string stream to read_json to parse the response.
		boost::property_tree::ptree pt;
		boost::property_tree::read_json(stream, pt);
		std::string result = pt.get<std::string>("result");

		return boost::lexical_cast<bool>(result);
	}

	point Automat::get_cursor_pos(void)
	{
		point coordinates;
		GetCursorPos(&coordinates);

		return coordinates;
	}

	point Automat::get_resolution(void)
	{
		// See https://stackoverflow.com/a/8690641/8374167.

		point coordinates;
		RECT desktop;
		HWND hDesktop;
		
		hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);

		coordinates.x = desktop.right;
		coordinates.y = desktop.bottom;

		return coordinates;
	}
}
