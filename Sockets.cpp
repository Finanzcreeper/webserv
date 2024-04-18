
#include "Sockets.hpp"

#include <unistd.h>

Sockets::Sockets() : client_address_length(sizeof(client_address)){
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8088);
	server_address.sin_addr.s_addr = INADDR_ANY;
	listening_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listening_socket == -1) {
		//throw socket creation failure exception
		std::cerr << "socket creation failure" << std::endl;
	}
	if (bind(listening_socket, (struct sockaddr *) &server_address, sizeof(server_address))== -1) {
		// throw socked binding failure exception
		std::cerr << "socked binding failure" << std::endl;
	}
	if (listen(listening_socket,1000) == -1) {
		//throw socket listening error exception
		std::cerr << "error making the socket listen" << std::endl;
	}
}

void Sockets::CheckForConnections() {
	listening_sockets[0].fd = listening_socket;
		std::cout << "listening on socket: " << listening_sockets[0].fd << std::endl;
		int i = 0;
		while (poll(listening_sockets,1,100) <= 0) {
			std::cout << "socket: "<< listening_sockets[0].fd << " has "<< i <<" connections in queue: " << std::endl;
		}


		if (poll(listening_sockets,1,-1) < 0) {
			connections.push_back(accept(listening_socket,(sockaddr *) &client_address, &client_address_length));
			int test = accept(listening_socket,(sockaddr *) &client_address, &client_address_length);
			write(test,"hello",5);
			std::cout << "connection accepted from: " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) <<std::endl;
		}
		std::cout << "end" << std::endl;
}
