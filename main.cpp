#include "server/Server.hpp"
#include <vector>
#include "parsers/ConfigParse.hpp"
#include <signal.h>

bool endserver = false;

void WebKill(int signal) {
	if (signal == SIGINT) {
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

	for (size_t i = 0; i < Config.size(); ++i){
		for (size_t j = i + 1; j < Config.size(); ++j) {
			if (Config[i].host == Config[j].host && Config[i].port == Config[j].port && Config[i].serverName == Config[j].serverName){
				std::vector<s_server>::iterator it;
				it = Config.begin() + static_cast<long>((j));
				Config.erase(it);
				--j;
			}
		}
	}



	for (size_t i = 0; i < Config.size(); ++i) {
		try {
			Servers.push_back(new Server(Config[i]));
			std::cout << "\033[1;32mServer: " << Config[i].host << ":" << Config[i].port << " created!\033[0m" << std::endl;
			std::cout << "-------------------------------------------------------------------------" << std::endl;
		} catch (std::runtime_error &e) {
			std::cout << "\033[1;31mServer: " << Config[i].host << ":" << Config[i].port << " not created!\033[0m" << std::endl;
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
