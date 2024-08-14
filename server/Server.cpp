
#include "Server.hpp"
#include <cerrno>
#include <cstring>
#include "../parsers/httpParser.hpp"
#include "../interpreters/httpInterpreter.hpp"
#include <unistd.h>
#include "MethodExecutor.hpp"
#include <fcntl.h>

#include <exception>

Server::Server(t_server sett) : settings(sett), socketOption(ON){
	this->lastTimeoutCheck = time(NULL);
	bzero(&listening_socket, sizeof(listening_socket));
	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(settings.host.c_str(),settings.port.c_str(),&hints,&serverInfo) != 0) {
		std::cerr << "\033[1;31mgetaddrinfo failed: " << std::strerror(errno) << "\033[0m" << std::endl;
		throw std::runtime_error("");
		//throw getaddrinfo exception
	}

	listening_socket.fd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
	if (listening_socket.fd == -1) {
		std::cerr << "\033[1;31msocket creation failure: "<< std::strerror(errno)  << "\033[0m"<< std::endl;
		freeaddrinfo(serverInfo);
		throw std::runtime_error("");
		//throw socket creation failure exception
	}

	fcntl(listening_socket.fd, O_NONBLOCK);

	int error;
	error = setsockopt(listening_socket.fd, SOL_SOCKET, SO_REUSEPORT, &socketOption, sizeof(socketOption));
	if (error == -1) {
		std::cerr << "socket option could not be applied: " << std::strerror(errno) << std::endl;
		//throw socket settings exception
	}

	error = setsockopt(listening_socket.fd, SOL_SOCKET, SO_REUSEADDR, &socketOption, sizeof(socketOption));
	if (error == -1) {
		std::cerr << "socket option could not be applied: " << std::strerror(errno) << std::endl;
	if (setsockopt(listening_socket.fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &socketOption, sizeof(socketOption)) == -1) {
		std::cerr << "\033[1;31msocket option could not be applied: " << std::strerror(errno)  << "\033[0m"<< std::endl;
		freeaddrinfo(serverInfo);
		throw std::runtime_error("");
		//throw socket settings exception
	}

	if (bind(listening_socket.fd, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1) {
		std::cerr << "\033[1;31msocked binding failure: "<< std::strerror(errno)  << "\033[0m"<< std::endl;
		freeaddrinfo(serverInfo);
		throw std::runtime_error("");
		// throw socked binding failure exception
	}

	if (listen(listening_socket.fd, 1000) == -1) {
		std::cerr << "\033[1;31merror making the socket listen: "<< std::strerror(errno)  << "\033[0m"<< std::endl;
		freeaddrinfo(serverInfo);
		throw std::runtime_error("");
		//throw socket listening error exception
	}
	setUpServer();
}

void Server::CheckForConnections() {
void Server::setUpServer() {

	executor = MethodExecutor(&(this->settings));

	request.r.requestCompletlyRecieved = false;

	request.r.ReqType = NONE;
	request.r.RequestIntegrity = OK_HTTP;

	listening_socket.events = POLLIN;
	Fds.push_back(listening_socket);

	std::cout << "listening on socket: " << Fds[0].fd << std::endl;
	int i = 0;
	while (true) {
 		i = poll(Fds.data(), Fds.size(), 0);
		//accepting new connection if there is one
		if ((Fds[0].revents & POLLIN)!= 0) {
			client.fd = accept(listening_socket.fd, NULL, NULL);
			if (client.fd == -1) {
				//throw accept connection exception
				std::cerr << "a connection failed: " << std::strerror(errno) << std::endl;
			}
			fcntl(client.fd,O_NONBLOCK);
			client.events = (POLLIN | POLLOUT);
			connectionMsgs.insert(std::make_pair(client.fd, request));
			answerMsgs.insert(std::make_pair(client.fd,response));
			Fds.push_back(client);
			connectionMsgs.find(client.fd)->second.t.msgAmt = 0;
			connectionMsgs.find(client.fd)->second.t.lastMsg = time(NULL);
			answerMsgs.find(client.fd)->second.isReady = true;
			--i;
		}
		//reading from connection (reading into buffers, or closing connections
		if (i > 0) {
			it = Fds.begin() + 1;
			mt = connectionMsgs.begin();
			while (it != Fds.end()) {
				if ((it->revents & POLLERR) != 0) {
					std::cout << "Socket error Occurred" << std::endl;
					cleanConnection();
				} else if ((it->revents & POLLHUP) != 0) {
					std::cout << "Socket hung up" << std::endl;
					cleanConnection();
				} else if ((it->revents & POLLIN) != 0) {
					bzero(buffer, sizeof(buffer));
					mt = connectionMsgs.find(it->fd);
					resps = answerMsgs.find(it->fd);
					this->recievedBytes = recv(it->fd, buffer, 1000, 0);
					if (this->recievedBytes > 0 ) {
						mt->second.r.RequestBuffer.append(buffer);
						time(&mt->second.t.lastMsg);
						++mt->second.t.msgAmt;
						if (mt->second.t.msgAmt > settings.timeoutReads) {
							mt->second.r.RequestIntegrity = REQUEST_TIMEOUT;
						}
						httpParser(mt);
						//std::cout << "after parser: " << mt->second.r.RequestIntegrity << std::endl;
						if (mt->second.r.requestCompletlyRecieved == true){
							interpretRequest(mt->second.r, settings);
							executor.wrapperRequest(mt->second.r, resps->second);
							mt->second.r.HeaderBuffer.clear();
							mt->second.r.RequestBuffer.clear();
							if (mt->second.r.HeaderFields.find("connection") != mt->second.r.HeaderFields.end()) {
								std::string connection = mt->second.r.HeaderFields.find("connection")->second;
								if (connection == "close") {
									std::cout << "client requested closing of connection" << std::endl;
									cleanConnection();
								}
							}
						}
					} else if (this->recievedBytes == 0) {
						std::cout << "client sent closed connection" << std::endl;
						cleanConnection();
					} else if (this->recievedBytes == -1) {
						revcErrorHandler();
					}
				} else if ((it->revents & POLLOUT) != 0 && answerMsgs.find(it->fd)->second.isReady) {
					resps = answerMsgs.find(it->fd);
					responder();
					mt->second.t.msgAmt = 0;
					resps->second.isReady = false;
				}
				if (mt->second.r.RequestIntegrity == REQUEST_TIMEOUT) {
					std::cout << "Client Timed out" << std::endl;
					cleanConnection();
				}
				++it;
			}
		}
	checkConnectionsForTimeout();
	}
}

void Server::cleanConnection() {
	std::cout << mt->first << " \033[1;31mdisconnected\033[0m" << std::endl;
	close(mt->first);
	Fds.erase(it);
	connectionMsgs.erase(mt);
	--it;
}

void Server::revcErrorHandler() {
	std::cout << "\033[1;34m" << std::strerror(errno) << "\033[0m" << std::endl;
	if(errno == ECONNRESET) {
		std::cout << "hii" << std::endl;
		cleanConnection();
	}
	if (errno != EWOULDBLOCK || errno != EAGAIN) {
		return;
	}
}

void Server::responder() {
	int sentAmt = 0;

	sentAmt = send(resps->first,resps->second.responseBuffer.c_str(),resps->second.responseBuffer.size(),MSG_DONTWAIT);
	if (sentAmt == -1) {
		return;
	}
	resps->second.responseBuffer.erase(0,sentAmt);
}

void Server::checkConnectionsForTimeout() {
	std::map<int,connection>::iterator it = connectionMsgs.begin();


	while (it != connectionMsgs.end()) {
		if (difftime(time(NULL), it->second.t.lastMsg) > settings.timeoutTime) {
			it->second.r.RequestIntegrity = REQUEST_TIMEOUT;
		}
		++it;
	}
}

Server::~Server() {
	freeaddrinfo(serverInfo);
}

