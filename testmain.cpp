#include "Sockets.hpp"

int main() {
	Sockets *sock = new Sockets();

	sock->CheckForConnections();

	delete sock;
}