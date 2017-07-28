#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

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
		std::string value;

		// Use property tree structure to store key/value pairs.
		boost::property_tree::ptree pt;
		boost::property_tree::ptree params;

		for (auto &pair : arg_map) {
			// Store parameter names and values.
			params.put(pair.first, pair.second);

			// Store value for the hack described later.
			if (pair.first.compare("key") == 0) {
				value = pair.second;
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
		
		if (!value.empty()) {
			boost::algorithm::erase_all(json, " ");
			boost::algorithm::erase_all(json, "\n");
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
}
