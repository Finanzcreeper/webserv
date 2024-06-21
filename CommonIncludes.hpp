#ifndef WEBSERV_COMMONINCLUDES_HPP
#define WEBSERV_COMMONINCLUDES_HPP

#include"server/statusCodes.h"
#include <map>
#include <vector>
//going to be saved as a vector to save multiple servers inside of one config file

struct route {
	std::string	location;
	int			methods;
	std::string	httpRedirection;
	std::string root;
	std::string	Index;
	bool		dirListing;
	std::map<std::string, std::string>	cgi;
};

typedef struct s_server {
	std::string	 						port;
	std::string 						host;
	std::string							server_name;
	std::string 						default_error_page;
	long unsigned int					client_max_body_size;
	std::map<std::string,route>			routes;
	int									httpMethods;
	std::vector<std::string>			path;
	std::map<std::string, std::string>	cgi_extension;
	bool								dir_listing;
	std::string							dir_request_default; //default file if the request is a directory
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
	route UsedRoute;
	std::string BodyBuffer;
	std::string Body;
};

struct Response {
	std::string 	responseBuffer;
	std::string 	headerBuffer;
	statusCode		ResponseIntegrity;
	std::string 	body;
	bool			isReady;
};

#endif //WEBSERV_COMMONINCLUDES_HPP
