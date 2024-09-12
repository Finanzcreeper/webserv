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

void printLocation(std::map<std::string, location>::iterator& it){
	location& location = it->second;
	std::cout << "name: " << it->first << std::endl;
	std::cout << "\t" << "httpMethods: " << location.httpMethods << std::endl;
	std::cout << "\t" << "redirect: " << location.redirect << std::endl;
	std::cout << "\t" << "dirListing: " << location.dirListing << std::endl;
	std::cout << "\t" << "index: " << location.index << std::endl;
	std::cout << "\t" << "root: " << location.root << std::endl;
	std::cout << "\t" << "locationName: " << location.locationName << std::endl;
}

void printServer(t_server &server){
	std::cout << "port: " << server.port << std::endl;
	std::cout << "host: " << server.host << std::endl;
	std::cout << "serverName: " << server.serverName << std::endl;
	std::cout << "clientMaxBodySize: " << server.clientMaxBodySize << std::endl;
	std::cout << "timeoutTime: " << server.timeoutTime << std::endl;
	std::cout << "timeoutReads: " << server.timeoutReads << std::endl;
	std::map<statusCode, std::string>::iterator it;
	std::cout << "errorPages:" << std::endl;
	for (it = server.errorPages.begin(); it != server.errorPages.end(); it++){
		std::cout << "\t" << it->first << ": " << it->second << std::endl;
	}
	std::map<std::string, location>::iterator it2;
	std::cout << "locations:" << std::endl;
	for (it2 = server.locations.begin(); it2 != server.locations.end(); it2++){
		printLocation(it2);
	}
	std::cout << "***finished***" << std::endl;
}


std::string							port;
	std::string							host;
	std::string							serverName;
	std::map<statusCode, std::string>	errorPages;
	long unsigned int					clientMaxBodySize;
	int									timeoutTime;
	unsigned int						timeoutReads;
	std::map<std::string, location>		locations;

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
	for (size_t i = 0; i < Config.size(); i++){
		printServer(Config[i]);
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
