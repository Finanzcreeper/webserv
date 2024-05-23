#include "Server.hpp"
#include <vector>
//#include "WebservServerConfigParser"

int main() {
	//std::vector<WebservConfigStruct> configs;
	//std::vector<WebservConfigStruct>::iterator servernbr = configs.begin();

	//while (servernbr != configs.end()) {
	struct WebservConfigStruct servernbr;

	servernbr.host = "127.0.0.1";
	servernbr.port = "8080";
	servernbr.client_max_body_size = 50;
		Server *sock = new Server(servernbr);
		sock->CheckForConnections();
		//servernbr++;
	//}

	delete sock;
}