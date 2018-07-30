#define _CRT_SECURE_NO_WARNINGS
#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// function that creates a tring to be sent back to the client
std::string make_daytime_string()
{
	using namespace std; // for time_t, time and ctime
	time_t now = time(0); // store system time value 0
	return ctime(&now);
}

int main()
{
	try
	{
		boost::asio::io_service io_service;
		// this object is created to listen for new connections
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13));

		for (;;)
		{
			// this socket represents the connection to the client
			tcp::socket socket(io_service);
			// wait for connection
			acceptor.accept(socket);

			// client connects to our service. determine current time 
			// and tranfer info to client 
			std::string message = make_daytime_string();

			boost::system::error_code ignored_error;
			boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}