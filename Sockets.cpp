
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
	std::vector<pollfd>::iterator it;

	listening_socket.clientPollFd.events = POLLIN;
	addToSocketArray(listening_socket);

		std::cout << "listening on socket: " << connectionFds[0].fd << std::endl;
		int i = 0;
		while (true) {
			i = poll(connectionFds.data(), connectionFds.size(), 0);
			if (connectionFds[0].revents != 0) {
				client.clientPollFd.fd = accept(listening_socket.clientPollFd.fd, (sockaddr *) &client.clientSocketAddress, &client_address_length);
				if (client.clientPollFd.fd == -1) {
					//throw accept connection exception
					std::cerr << "connection failed: " << std::strerror(errno) << std::endl;
				}
				client.clientPollFd.events = (POLLIN);
				addToSocketArray(client);



				write(client.clientPollFd.fd, "hello", 5);
				std::cout << "connection accepted from: " << inet_ntoa(client.clientSocketAddress.sin_addr) << ":" << ntohs(client.clientSocketAddress.sin_port) <<std::endl;
				std::cout << "active connections: " << connectionFds.size() << std::endl;
				it = connectionFds.begin();
				f = 0;
				while (it != connectionFds.end()) {
					std::cout << "connection nr " << f++ << " fd: " << it->fd << std::endl;
					it++;
				}
				--i;
			}
			if (i != 0) {
				//read from fds here!
				f = 1;
				while ((long unsigned)f != connectionFds.size()) {
					if (connectionFds[f].revents == POLLIN){
						bzero(buffer, sizeof(buffer));
						recv(connectionFds[f].fd, buffer,1000,0);
						connectionMsgs[f] = connectionMsgs[f] + buffer;
					}
					f++;
				}
			}
			if (connectionMsgs.size() > 1 && connectionFds[1].revents == 1) {
				std::cout << connectionMsgs[1] << std::endl;
			}
		}
		std::cout << "end" << std::endl;
}

void Sockets::addToSocketArray(AllSockets insert) {
	connectionAddrs.push_back(insert.clientSocketAddress);
	connectionFds.push_back(insert.clientPollFd);
	connectionMsgs.push_back("");
}

void Sockets::removeFromSocketArray(unsigned int pos) {
	if (pos <= connectionFds.size()) {
		connectionAddrs.erase(connectionAddrs.begin() + pos);
		connectionFds.erase((connectionFds.begin() + pos));
		connectionMsgs.erase((connectionMsgs.begin() + pos));
	}
}

Sockets::~Sockets() {
	freeaddrinfo(serverInfo);
}


