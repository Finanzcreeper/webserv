#ifndef WEBSERV_SOCKETS_HPP
#define WEBSERV_SOCKETS_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <vector>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>

enum sockoption{
	OFF,
	ON,
};

struct AllSockets{
	sockaddr_in clientSocketAddress;
	pollfd clientPollFd;
};

/*
 * On construction, create one listening socket.
 * have a public container containing connected sockets.
 * have one public function that uses poll to check if new connections have arrived in queue.
 * if new connections are in queue, accept them and put them into container.
*/

class Sockets {
private:
	int socketOption;

	addrinfo hints;
	addrinfo *serverInfo;

	AllSockets listening_socket;

	AllSockets client;
	socklen_t client_address_length;

	std::vector<sockaddr_in>connectionAddrs;
	std::vector<pollfd>connectionFds;

	void addToSocketArray(AllSockets insert);

	void removeFromSocketArray(unsigned int pos);

public:
	Sockets();
	void CheckForConnections();
	~Sockets();
};


#endif //WEBSERV_SOCKETS_HPP
