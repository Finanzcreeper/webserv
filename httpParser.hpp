#ifndef HTTPPARSER_HPP
#define HTTPPARSER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <vector>
#include <map>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <algorithm>
#include "Server.hpp"
#include "CommonIncludes.hpp"

//struct Request;

class httpParser {
private:
	std::map<int,Request>::iterator& req;
	const t_server settings;

	void GetRequestedPath(Request& request);
	void GetRequestType(Request& request);
	void beheader(Request& request);
	void decapitalizeHeaderFields(std::string& Header);
	void extractHeaderFields(Request& req);
	void handleBody(Request& body);
public:
		httpParser(std::map<int, Request>::iterator& req, t_server sett);

	//httpParser(std::map<int, Request>::iterator& req, const t_server sett);
};



#endif //HTTPPARSER_HPP
