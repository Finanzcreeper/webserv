#ifndef WEBSERV_HTTPPARSER_HPP
#define WEBSERV_HTTPPARSER_HPP

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

//struct Request;

class httpParser {
private:
	std::pair<int, Request> req;
	const struct WebservConfigStruct settings;

	void GetRequestedPath(Request& request);
	void GetRequestType(Request& request);
	void beheader(Request& request);
	void decapitalizeHeaderFields(std::string& Header);
	void extractHeaderFields(Request& req);
	void handleChunked(Request& body);
public:
	httpParser(std::map<int, Request>::iterator& req, struct WebservConfigStruct sett);

	//httpParser(std::map<int, Request>::iterator& req, const WebservConfigStruct sett);
};



#endif //WEBSERV_HTTPPARSER_HPP
