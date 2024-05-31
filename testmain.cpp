#include "Server.hpp"
#include <vector>
#include "config/ConfigParse.hpp"
//#include "WebservServerConfigParser"

int main() {
	//std::vector<t_server> configs;
	//std::vector<t_server>::iterator servernbr = configs.begin();
	std::vector<t_server> Config;
	try  {
		Config = configParse("config/sampleConfig.conf");
	} catch (std::runtime_error &e) {
		std::cout <<e.what() << std::endl;
	}
	t_server sett;
	Config.push_back(sett);
	//while (servernbr != configs.end()) {
	Config[0].host = "127.0.0.1";
	Config[0].port = "8080";
	Config[0].client_max_body_size = 50;
	Config[0].httpMethods = GET | POST;
		Server *sock = new Server(Config[0]);
		sock->CheckForConnections();
		//servernbr++;
	//}

	delete sock;
}