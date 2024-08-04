#include "server/Server.hpp"
#include <vector>
#include "parsers/ConfigParse.hpp"

int main(int argn, char *argv[]) {
	std::vector<t_server>	Config;
	std::string				path;
	if (argn == 2)
		path = std::string(argv[1]);
	else
		path = std::string("parsers/sampleConfig.conf");

	try {
		Config = configParse(path);
	} 
	catch (std::runtime_error &e) {
		std::cout <<e.what() << std::endl;
		return (1);
	}
	Server *sock = new Server(Config[0]);
	sock->CheckForConnections();
	//servernbr++;
	//}

	delete sock;
}
