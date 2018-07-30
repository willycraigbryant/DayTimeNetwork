#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage: client <host>" << std::endl;
			return 1;
		}

		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service); // turn into TCP endpoint (connecting out to)
		tcp::resolver::query query(argv[1], "daytime"); // searching for the IP address of the server 
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query); // list of endpoints is returned using an iterator 

																			 // create and connect the sockets. Since the endpoints above contain IPv4 and IPv6 endpoints, we 
																			 // try each of them until one works. connect() will do this automatically 
		tcp::socket socket(io_service);
		boost::asio::connect(socket, endpoint_iterator);

		// now the connection is open. We now need to read the response from the daytime service

		// boost array holds and receives data. buffer() automatically determines the size of
		// the array to help prevent buffer overruns
		for (;;)
		{
			boost::array<char, 128> buf;
			boost::system::error_code error;

			// when the server closes the connection, read_some() will exit with an error (:::eof),
			// which is how we know to exit the loop
			size_t len = socket.read_some(boost::asio::buffer(buf), error);

			if (error == boost::asio::error::eof)
				break; // connection closed cleanly by peer
			else if (error)
				throw boost::system::system_error(error); // some other error

			std::cout.write(buf.data(), len);
		}
	}
	// handle any other exceptions
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
