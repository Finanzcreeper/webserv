
#include "Server.hpp"
#include <cerrno>
#include <cstring>
#include "../parsers/httpParser.hpp"
#include "../interpreters/httpInterpreter.hpp"
#include <unistd.h>
#include "MethodExecutor.hpp"
#include <fcntl.h>

#include <exception>

Server::Server(t_server sett) : settings(sett), socketOption(ON) {
	this->lastTimeoutCheck = time(NULL);
	bzero(&listening_socket, sizeof(listening_socket));
	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(settings.host.c_str(), settings.port.c_str(), &hints, &serverInfo) != 0) {
		std::cerr << "\033[1;31mgetaddrinfo failed: " << std::strerror(errno) << "\033[0m" << std::endl;
		throw std::runtime_error("");
		//throw getaddrinfo exception
	}

	listening_socket.fd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
	if (listening_socket.fd == -1) {
		std::cerr << "\033[1;31msocket creation failure: " << std::strerror(errno) << "\033[0m" << std::endl;
		freeaddrinfo(serverInfo);
		throw std::runtime_error("");
		//throw socket creation failure exception
	}

	int error = 0;
	error = setsockopt(listening_socket.fd, SOL_SOCKET, SO_REUSEPORT, &socketOption, sizeof(socketOption));
	if (error == -1) {
		std::cerr << "socket option could not be applied: " << std::strerror(errno) << std::endl;
		throw std::runtime_error("");
		//throw socket settings exception
	}

	error = setsockopt(listening_socket.fd, SOL_SOCKET, SO_REUSEADDR, &socketOption, sizeof(socketOption));
	if (error == -1) {
		std::cerr << "socket option could not be applied: " << std::strerror(errno) << std::endl;
		throw std::runtime_error("");
		//throw socket settings exception
	}

	if (bind(listening_socket.fd, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1) {
		std::cerr << "\033[1;31msocked binding failure: " << std::strerror(errno) << "\033[0m" << std::endl;
		freeaddrinfo(serverInfo);
		throw std::runtime_error("");
		// throw socked binding failure exception
	}

	if (listen(listening_socket.fd, 10000) == -1) {
		std::cerr << "\033[1;31merror making the socket listen: " << std::strerror(errno) << "\033[0m" << std::endl;
		freeaddrinfo(serverInfo);
		throw std::runtime_error("");
		//throw socket listening error exception
	}

	fcntl(listening_socket.fd,F_SETFL ,O_NONBLOCK);
	setUpServer();
	connAMT = 0;
}

void Server::setUpServer() {

	executor = MethodExecutor(&(this->settings));

	request.r.requestCompletlyRecieved = false;

	request.r.ReqType = NONE;
	request.r.RequestIntegrity = OK_HTTP;

	response.responseBuffer.clear();
	response.isReady = false;
	response.isDone = false;
	response.body.clear();
	response.headerFields.clear();
	response.httpStatus = OK_HTTP;

	listening_socket.events = POLLIN;
	Fds.push_back(listening_socket);

	connectionAmount = 0;
}


void Server::CheckForConnections() {
	connectionAmount = poll(Fds.data(), Fds.size(), 0);
	 //accepting new connection if there is one
	if ((Fds[0].revents & POLLIN)!= 0) {
		client.fd = accept(listening_socket.fd, NULL, NULL);
		if (client.fd != -1) {
			client.events = (POLLIN | POLLOUT);
			client.revents = 0;
			connectionMsgs.insert(std::make_pair(client.fd, request));
			answerMsgs.insert(std::make_pair(client.fd,response));
			Fds.push_back(client);
			connectionMsgs.find(client.fd)->second.t.msgAmt = 0;
			connectionMsgs.find(client.fd)->second.t.lastMsg = time(NULL);
			--connectionAmount;
		}
	}
	//reading from connection (reading into buffers, or closing connections
	if (connectionAmount > 0) {
		it = Fds.begin() + 1;
		mt = connectionMsgs.begin();
		resps = answerMsgs.begin();
		while (it != Fds.end()) {
			mt = connectionMsgs.find(it->fd);
			resps = answerMsgs.find(it->fd);
			if ((it->revents & POLLERR) != 0) {
				cleanConnection();
			} else if ((it->revents & POLLHUP) != 0) {
				cleanConnection();
			} else if ((it->revents & POLLIN) != 0) {
				bzero(buffer, sizeof(buffer));
				this->recievedBytes = recv(it->fd, buffer, 1000, 0);
				if (this->recievedBytes > 0 ) {
					mt->second.r.RequestBuffer.append(buffer);
					time(&mt->second.t.lastMsg);
					++mt->second.t.msgAmt;
					if (mt->second.t.msgAmt > settings.timeoutReads) {
						mt->second.r.RequestIntegrity = REQUEST_TIMEOUT;
					}
					httpParser(mt);
					if (mt->second.r.requestCompletlyRecieved == true){
						interpretRequest(mt->second.r, settings);
						executor.wrapperRequest(mt->second.r, resps->second);
						mt->second.r.HeaderBuffer.clear();
						mt->second.r.RequestBuffer.clear();
						mt->second.r.Body.clear();
					}
				} else if (this->recievedBytes == 0) {
					cleanConnection();
				} else if (this->recievedBytes == -1) {
					cleanConnection();
				}
			} else if ((it->revents & POLLOUT) != 0 &&  resps->second.isReady == true) {
				responder();
				if (resps->second.isDone == true) {
					mt->second.t.msgAmt = 0;
					resps->second.isReady = false;
					if (mt->second.r.HeaderFields.find("connection") != mt->second.r.HeaderFields.end()) {
						std::string connection = mt->second.r.HeaderFields.find("connection")->second;
						if (connection == "close") {
							cleanConnection();
						}
					}
				}
			}
			++it;
		}
	}
checkConnectionsForTimeout();
}

void Server::cleanConnection() {
	close(mt->first);
	Fds.erase(it);
	connectionMsgs.erase(mt);
	answerMsgs.erase(resps);
	--it;
}

void Server::responder() {
	int sentAmt = 0;

	sentAmt = send(resps->first,resps->second.responseBuffer.c_str(),resps->second.responseBuffer.size(),MSG_DONTWAIT);
	if (sentAmt == 0) {
		return;
	}
	if (sentAmt == -1) {
		cleanConnection();
		return;
	}
	if (mt->second.r.RequestIntegrity == REQUEST_TIMEOUT) {
		cleanConnection();
		return;
	}
	resps->second.responseBuffer.erase(0,sentAmt);
	if (resps->second.responseBuffer.empty() == true) {
		resps->second.isDone = true;
	}
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

void Server::CleanFds() {
	std::vector<pollfd>::iterator it;
	it = Fds.begin();
	while (it != Fds.end()) {
		close (it->fd);
		++it;
	}
}

Server::~Server() {
	freeaddrinfo(serverInfo);
}

