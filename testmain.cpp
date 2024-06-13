#include "server/Server.hpp"
#include <vector>
#include "parsers/ConfigParse.hpp"
#include <cstdlib>

int main() {
	//std::vector<t_server> configs;
	//std::vector<t_server>::iterator servernbr = configs.begin();
	std::vector<t_server> Config;
	try  {
		Config = configParse("parsers/sampleConfig.conf");
	} catch (std::runtime_error &e) {
		std::cout <<e.what() << std::endl;
	}
		Config[0].host = "127.0.0.1";
		location dummyLocation;
		dummyLocation._httpMethods = GET & POST & HEAD;
		// dummyLocation._errorPage = 
		dummyLocation._path = std::getenv("OLDPWD") + std::string("/content");
		// dummyLocation._redirect =
		Config[0].locations["/"] = dummyLocation;
		Server *sock = new Server(Config[0]);
		sock->CheckForConnections();
		//servernbr++;
	//}

	delete sock;
}