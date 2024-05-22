
#include "Server.hpp"
#include <cerrno>
#include <cstring>
#include "httpParser.hpp"
#include <unistd.h>

Server::Server(WebservConfigStruct settings) : socketOption(ON){
	bzero(&listening_socket, sizeof(listening_socket));
	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(settings.host.c_str(),settings.port.c_str(),&hints,&serverInfo) != 0) {
		//throw getaddrinfo exception
		std::cerr << "getaddrinfo failed: " << std::strerror(errno) << std::endl;
	}

	listening_socket.fd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
	if (listening_socket.fd == -1) {
		//throw socket creation failure exception
		std::cerr << "socket creation failure: "<< std::strerror(errno) << std::endl;
	}

	if (setsockopt(listening_socket.fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &socketOption, sizeof(socketOption)) == -1) {
		std::cerr << "socket option could not be applied: " << std::strerror(errno) << std::endl;
		//throw socket settings exception
	}

	if (bind(listening_socket.fd, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1) {
		// throw socked binding failure exception
		std::cerr << "socked binding failure: "<< std::strerror(errno) << std::endl;
	}

	if (listen(listening_socket.fd, 1000) == -1) {
		//throw socket listening error exception
		std::cerr << "error making the socket listen: "<< std::strerror(errno) << std::endl;
	}
}

void Server::CheckForConnections() {
	std::vector<pollfd>::iterator it;
	std::map<int, Request>::iterator mt;

	Request Request;

	listening_socket.events = POLLIN;
	Fds.push_back(listening_socket);

	std::cout << "listening on socket: " << Fds[0].fd << std::endl;
	int i = 0;
	while (true) {
		i = poll(Fds.data(), Fds.size(), 0);
		//accepting new connection if there is one
		if (Fds[0].revents != 0) {
			client.fd = accept(listening_socket.fd, NULL, NULL);
			if (client.fd == -1) {
				//throw accept connection exception
				std::cerr << "a connection failed: " << std::strerror(errno) << std::endl;
			}
			client.events = (POLLIN);
			connectionMsgs.insert(std::make_pair(client.fd,Request));
			Fds.push_back(client);
			--i;
		}
		//reading from connection (reading into buffers, or closing connections
		if (i > 0) {
			it = Fds.begin();
			mt = connectionMsgs.begin();
			while (it != Fds.end()) {
				if (it->revents == POLLIN){
					bzero(buffer, sizeof(buffer));
					while (mt->first != it->fd) {
						++mt;
					}
					if (recv(it->fd, buffer, 1000, 0) != 0) {
						mt->second.RequestBuffer.append(buffer);
						httpParser(*mt);
					} else {
						//cleanup
						std::cout << mt->first << " disconnected" << std::endl;
						Fds.erase(it);
						connectionMsgs.erase(mt);
						--it;
					}
				}
				++it;
			}
		}
	}
	std::cout << "end" << std::endl;
}

	/*
	 * Messages have structure:
	 * first line is message type and path
	 * after first line header begins
	 *
	 * header options:
	 *	1: no hint to message -> no message
	 *	\r\n\r\n ends message;
	 *		Solution:
	 *		1: send 411 code
	 *		2: handle situation
	 *
	 *	2: Header contains Transfer-Encoding: chunked, ...
	 *	multiple messages will be sent, each containing size of message in OCTET
	 *	message follows after
	 *	message chunks look like:
	 *
	 *	<OCTAL number>\r\n<message>\r\n
	 *
	 *	<message> MUST be the correct length
	 *
	 *	the last chunk is defined as
	 *
	 *	0\r\n\r\n
	 *		Solution:
	 *		handle chunked messages when Transfer-Encoding: chunked, ... is found
	 *
	 *	3: Header contains Content-Length: ...
	 *		Solution: message is exactly ... bytes long, read that amount in total
	 *
	 *	4: Header contains Transfer-Encoding: and Content-Length:
	 *		Solution: ignore Content-Length and apply Transfer-Encoding Rule.
	 *
	 *
	 *
	 * options after the header:
	 *
	 *
	 *
	 * Solution:
	 *
	 * when empty
	 *
	 * header ends with \r\n\r\n
	*/



Server::~Server() {
	freeaddrinfo(serverInfo);
}


