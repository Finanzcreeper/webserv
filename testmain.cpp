#include "server/Server.hpp"
#include <vector>
#include "parsers/ConfigParse.hpp"
#include <cstdlib>

int main() {
	//std::vector<t_server> configs;
	//std::vector<t_server>::iterator servernbr = configs.begin();
	std::vector<t_server> Config;
	try  {
		Config = configParse(std::getenv("PWD") + std::string("/parsers/sampleConfig.conf"));
	} catch (std::runtime_error &e) {
		std::cout <<e.what() << std::endl;
	}
		Config[0].workingDir = std::getenv("PWD");
		Server *sock = new Server(Config[0]);
		sock->CheckForConnections();
		//servernbr++;
	//}

	delete sock;
}