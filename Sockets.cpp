
#include "Sockets.hpp"
#include <cerrno>
#include <cstring>
#include <unistd.h>

Sockets::Sockets() : socketOption(ON){
	bzero(&listening_socket, sizeof(listening_socket));
	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL,"8080",&hints,&serverInfo) != 0) {
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

void Sockets::CheckForConnections() {
	std::vector<pollfd>::iterator it;
	std::map<int, std::string>::iterator mt;

	listening_socket.events = POLLIN;
	Fds.push_back(listening_socket);

		std::cout << "listening on socket: " << Fds[0].fd << std::endl;
		int i = 0;
		while (true) {
			i = poll(Fds.data(), Fds.size(), 0);
			if (Fds[0].revents != 0) {
				client.fd = accept(listening_socket.fd, NULL, NULL);
				if (client.fd == -1) {
					//throw accept connection exception
					std::cerr << "a connection failed: " << std::strerror(errno) << std::endl;
				}
				client.events = (POLLIN);
				connectionMsgs.insert(std::make_pair(client.fd,""));
				Fds.push_back(client);
				--i;
			}
			if (i != 0) {
				it = Fds.begin();
				mt = connectionMsgs.begin();
				while (it != Fds.end()) {
					if (it->revents == POLLIN){
						bzero(buffer, sizeof(buffer));
						while (mt->first != it->fd) {
							++mt;
						}
						if (recv(it->fd, buffer, 1000, 0) != 0) {
							mt->second.append(buffer);
							std::cout << mt->second << std::endl;
						} else {
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

Sockets::~Sockets() {
	freeaddrinfo(serverInfo);
}

