#include "server/Server.hpp"
#include <vector>
#include "parsers/ConfigParse.hpp"
#include <cstdlib>

int main() {
	//std::vector<t_server> configs;
	//std::vector<t_server>::iterator servernbr = configs.begin();
	std::vector<t_server> Config;
	try  {
		Config = configParse("/home/nreher/dev/webserv/parsers/sampleConfig.conf");
	} catch (std::runtime_error &e) {
		std::cout <<e.what() << std::endl;
	}
		Config[0].host = "127.0.0.1";
		Config[0].port = "8080";
		Config[0].serverName = "webserv";
		location dummyLocation;
		dummyLocation.httpMethods = GET | POST | HEAD;
		dummyLocation.dirListing = true;
		dummyLocation.root = std::getenv("OLDPWD") + std::string("/content");
		Config[0].workingDir = std::getenv("OLDPWD");
		Config[0].locations["/"] = dummyLocation;
		Server *sock = new Server(Config[0]);
		sock->CheckForConnections();
		//servernbr++;
	//}

	delete sock;
}