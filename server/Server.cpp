
#include "Server.hpp"
#include <cerrno>
#include <cstring>
#include "../parsers/httpParser.hpp"
#include "../interpreters/httpInterpreter.hpp"
#include <unistd.h>
#include "MethodExecutor.hpp"

Server::Server(t_server sett) : settings(sett), socketOption(ON){
	this->lastTimeoutCheck = time(NULL);
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
	std::map<int, connection>::iterator mt;
	std::map<int, Response>::iterator resps;

	MethodExecutor executor = MethodExecutor(&(this->settings));

	connection request;
	request.r.requestCompletlyRecieved = false;
	Response response;

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
				if ((it->revents & POLLIN) != 0){
					bzero(buffer, sizeof(buffer));
					mt = connectionMsgs.find(it->fd);
					resps = answerMsgs.find(it->fd);
					if (recv(it->fd, buffer, 1000, 0) != 0) {
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
							mt->second.r.requestCompletlyRecieved = false;
						}
					} else {
						//cleanup
						std::cout << mt->first << " disconnected" << std::endl;
						Fds.erase(it);
						connectionMsgs.erase(mt);
						--it;
					}
				}
				//int b = 0;
				if ((it->revents & POLLOUT) != 0 && answerMsgs.find(it->fd)->second.isReady) {
					resps = answerMsgs.find(it->fd);
					send(it->fd, resps->second.responseBuffer.c_str(), resps->second.responseBuffer.length(), 0);
					resps->second.isReady = false;
				}
				++it;
			}
		}
		if (difftime(time(NULL), lastTimeoutCheck) > settings.timeoutTime) {
		checkConnectionsForTimeout();
		}
	}
}

void Server::responder(std::map <int, Response>::iterator& response) {
	int sentAmt = 0;

	std::map <int, Response>::iterator& re = response;
	sentAmt = send(re->first,re->second.responseBuffer.c_str(),re->second.responseBuffer.size(),MSG_DONTWAIT);
	if (sentAmt == -1) {
		return;
	}
		response->second.responseBuffer.erase(0,sentAmt);
}

void Server::checkConnectionsForTimeout() {
	std::map<int,connection>::iterator it = connectionMsgs.begin();

	while (it != connectionMsgs.end()) {
		if (difftime(it->second.t.lastMsg,time(NULL)) > settings.timeoutTime) {
			it->second.r.RequestIntegrity = REQUEST_TIMEOUT;
		}
		++it;
	}
}

Server::~Server() {
	freeaddrinfo(serverInfo);
}


