#ifndef WEBSERV_COMMONINCLUDES_HPP
#define WEBSERV_COMMONINCLUDES_HPP

#include <vector>
#include <map>
#include"server/statusCodes.h"
#include <ctime>

#define HTTP_PROTOCOL "HTTP/1.1"

struct location {
	int									httpMethods;
	std::string							redirect;
	bool								dirListing;
	std::string							index;
	std::map<std::string, std::string>	cgi;
	std::string							root;
	std::string	 						locationName;
};

typedef struct s_server {
	std::string							port;
	std::string							host;
	std::string							serverName;
	std::map<statusCode, std::string>	errorPages;
	long unsigned int					clientMaxBodySize;
	int									timeoutTime;
	unsigned int						timeoutReads;
	std::map<std::string, location>		locations;
}t_server;

typedef enum Requesttype {
	NONE = 1 << 0,		//1
	INVALID = 1 << 1,	//2
	GET = 1 << 2,		//4
	HEAD = 1 << 3,		//8
	POST = 1 << 4,		//16
	PUT = 1 << 5,		//32
	DELETE = 1 << 6,	//64
	CONNECT = 1 << 7,	//128
	OPTIONS = 1 << 8,	//256
	TRACE = 1 << 9,		//512
	PATCH = 1 << 10,	//1204
} RequestType;

enum sockoption{
	OFF,
	ON,
};

struct Request {
	std::string RequestBuffer;
	std::string HeaderBuffer;
	std::map<std::string,std::string> HeaderFields;
	RequestType ReqType;
	statusCode RequestIntegrity;
	std::string RequestedPath;
	std::string RoutedPath;
	std::string BodyBuffer;
	location	UsedRoute;
	std::string Body;
	bool requestCompletlyRecieved;
};

struct Response {
	std::string 						responseBuffer;
	std::map<std::string,std::string>	headerFields;
	statusCode							httpStatus;
	std::string 						body;
	bool								isReady;
};

struct timeoutHandler {
	time_t lastMsg;
	unsigned int msgAmt;
};

struct connection {
	timeoutHandler t;
	Request r;
};

char	**prepareEnvVariables(Request& requ);
void	executeCGI(std::string programPath, Request& requ, Response& resp);

#endif //WEBSERV_COMMONINCLUDES_HPP
