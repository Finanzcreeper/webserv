#ifndef WEBSERV_SOCKETS_HPP
#define WEBSERV_SOCKETS_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <vector>
#include <arpa/inet.h>

enum sockoption{
	OFF,
	ON,
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
	int listening_socket;
	sockaddr_in server_address;
	pollfd listening_sockets[1];

	sockaddr_in client_address;
	socklen_t client_address_length;

	std::vector<int>connections;


public:
	Sockets();
	void CheckForConnections();
};


#endif //WEBSERV_SOCKETS_HPP
