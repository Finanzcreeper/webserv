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
//#include "../server/Server.hpp"
#include "../CommonIncludes.hpp"


	void httpParser(std::map<int, connection>::iterator& req);
	void GetRequestedPath(Request& request);
	void GetRequestType(Request& request);
	void decapitalizeHeaderFields(std::string& Header);
	void extractHeaderFields(Request& req);
	void handleHeader(Request &request, size_t endOfBlock);
	void handleBody(Request &request, size_t endOfBlock);
	size_t findEndOfBlock(std::string buffer);

#endif //HTTPPARSER_HPP
