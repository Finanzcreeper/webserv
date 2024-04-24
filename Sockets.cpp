
#include "Sockets.hpp"
#include <cerrno>
#include <cstring>
#include <unistd.h>

Sockets::Sockets() : socketOption(ON), client_address_length(sizeof(client.clientSocketAddress)){
	bzero(&listening_socket, sizeof(listening_socket));
	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL,"8080",&hints,&serverInfo) != 0) {
		//throw getaddrinfo exception
		std::cerr << "getaddrinfo failed: " << std::strerror(errno) << std::endl;
	}

	listening_socket.clientPollFd.fd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
	if (listening_socket.clientPollFd.fd == -1) {
		//throw socket creation failure exception
		std::cerr << "socket creation failure: "<< std::strerror(errno) << std::endl;
	}

	if (setsockopt(listening_socket.clientPollFd.fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &socketOption, sizeof(socketOption)) == -1) {
		std::cerr << "socket option could not be applied: " << std::strerror(errno) << std::endl;
		//throw socket settings exception
	}

	if (bind(listening_socket.clientPollFd.fd, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1) {
		// throw socked binding failure exception
		std::cerr << "socked binding failure: "<< std::strerror(errno) << std::endl;
	}

	if (listen(listening_socket.clientPollFd.fd, 1000) == -1) {
		//throw socket listening error exception
		std::cerr << "error making the socket listen: "<< std::strerror(errno) << std::endl;
	}
}

void Sockets::CheckForConnections() {
	int f;
	std::vector<AllSockets>::iterator it;

	listening_socket.clientPollFd.events = POLLIN;
	connections.push_back(listening_socket);

		std::cout << "listening on socket: " << connections[0].clientPollFd.fd << std::endl;
		int i = 0;
		while (true) {
			i = poll(&(connections.data()->clientPollFd), connections.size(), 0);
			if (i > 0) {
				client.clientPollFd.fd = accept(listening_socket.clientPollFd.fd, (sockaddr *) &client.clientSocketAddress, &client_address_length);
				if (client.clientPollFd.fd == -1) {
					//throw accept connection exception
					std::cerr << "connection failed: " << std::strerror(errno) << std::endl;
				}
				client.clientPollFd.events = (POLLIN);
				connections.push_back(client);



				write(client.clientPollFd.fd, "hello", 5);
				std::cout << "connection accepted from: " << inet_ntoa(client.clientSocketAddress.sin_addr) << ":" << ntohs(client.clientSocketAddress.sin_port) <<std::endl;
				std::cout << "active connections: " << connections.size() << std::endl;
				it = connections.begin();
				f = 0;
				while (it != connections.end()) {
					std::cout << "connection nr " << f++ << " fd: " << it->clientPollFd.fd << std::endl;
					it++;
				}
			}
		}
		std::cout << "end" << std::endl;
}

Sockets::~Sockets() {
	freeaddrinfo(serverInfo);
}
