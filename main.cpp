#include "server/Server.hpp"
#include <vector>
#include "parsers/ConfigParse.hpp"
#include <signal.h>

bool endserver = false;

void WebKill(int signal) {
	if (signal > 0) {
		endserver = true;
	}
}


int main(int argn, char *argv[]) {
	std::vector<Server*> Servers;
	std::vector<Server*>::iterator ServerCrawler;
	std::vector<t_server>	Config;
 
	signal(SIGINT, WebKill);
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
	for (size_t i = 0; i < Config.size(); ++i) {
		try {
			Servers.push_back(new Server(Config[i]));
			std::cout << "-------------------------------------------------------------------------" << std::endl;
		} catch (std::runtime_error &e) {
			std::cout << "Server: " << Config[i].host << ":" << Config[i].port << " not created!" << std::endl;
			std::cout << "-------------------------------------------------------------------------" << std::endl;
		}
	}
	ServerCrawler = Servers.begin();
	if (Servers.empty() == true) {
		std::cerr <<"\033[1;31mNo servers got Created ... Exiting now!"  << "\033[0m" << std::endl;
		return(0);
	}
	while(endserver == false) {
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
