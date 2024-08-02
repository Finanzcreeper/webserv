#include "server/Server.hpp"
#include <vector>
#include "parsers/ConfigParse.hpp"

int main() {
	//std::vector<t_server> configs;
	//std::vector<t_server>::iterator servernbr = configs.begin();
	std::vector<t_server> Config;
	std::vector<Server*> Servers;
	std::vector<Server*>::iterator ServerCrawler;

	try  {
		Config = configParse("/home/nreher/dev/webserv/parsers/sampleConfig.conf");
	} catch (std::runtime_error &e) {
		std::cout <<e.what() << std::endl;
	}
	for (size_t i = 0; i < Config.size(); ++i) {
		Servers.push_back(new Server(Config[i]));
	}
	ServerCrawler = Servers.begin();
	while(true) {
		if (ServerCrawler == Servers.end()) {
			ServerCrawler = Servers.begin();
		}
		(*ServerCrawler)->CheckForConnections();
		++ServerCrawler;
	}
	ServerCrawler = Servers.begin();
	while ( ServerCrawler != Servers.end()) {
		delete *ServerCrawler;
		++ServerCrawler;
	}
}
