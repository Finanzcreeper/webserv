#include "Server.hpp"
#include <vector>
#include "config/ConfigParse.hpp"
//#include "WebservServerConfigParser"

int main() {
	//std::vector<t_server> configs;
	//std::vector<t_server>::iterator servernbr = configs.begin();
	std::vector<t_server> Config;
	try  {
		Config = configParse("/home/nreher/dev/webserv/config/sampleConfig.conf");
	} catch (std::runtime_error &e) {
		std::cout <<e.what() << std::endl;
	}
		Server *sock = new Server(Config[0]);
		sock->CheckForConnections();
		//servernbr++;
	//}

	delete sock;
}