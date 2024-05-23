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



struct Request;
void httpParser(std::pair<int, Request> pair);
void GetRequestedPath(Request& request);
void GetRequestType(Request& request);
void beheader(Request& request);
void decapitalizeHeaderFields(std::string& Header);
void extractHeaderFields(Request& req);
void handleChuncked(std::string body);


#endif //WEBSERV_HTTPPARSER_HPP
