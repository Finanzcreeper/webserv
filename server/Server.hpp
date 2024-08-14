#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <vector>
#include <map>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <algorithm>
#include <exception>
#include <ctime>

//#include "../parsers/ConfigParse.hpp"
#include "../CommonIncludes.hpp"
//#include "httpParser.hpp"

/*
 * On construction, create one listening socket.
 * have a public container containing connected sockets.
 * have one public function that uses poll to check if new connections have arrived in queue.
 * if new connections are in queue, accept them and put them into container.
*/

class Server {
private:

	const t_server settings;

	int socketOption;

	long int recievedBytes;

	addrinfo hints;
	addrinfo *serverInfo;

	pollfd listening_socket;

	pollfd client;
	//socklen_t client_address_length;

	char buffer[1000];

	time_t lastTimeoutCheck;

	std::vector<pollfd>Fds;
	std::map<int, connection>connectionMsgs;
	std::map<int, Response>answerMsgs;

	std::vector<pollfd>::iterator it;
	std::map<int, connection>::iterator mt;
	std::map<int, Response>::iterator resps;

	void responder();

	void checkConnectionsForTimeout();

	void cleanConnection();

	void revcErrorHandler();

public:
	Server(t_server sett);
	void CheckForConnections();
	~Server();
};


#endif //SERVER_HPP