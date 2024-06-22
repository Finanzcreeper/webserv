#ifndef WEBSERV_COMMONINCLUDES_HPP
#define WEBSERV_COMMONINCLUDES_HPP

#include <vector>
#include <map>
#include"server/statusCodes.h"

# define HTTP_PROTOCOL "HTTP/1.1"

//going to be saved as a vector to save multiple servers inside of one config file

// url would be looked like this
// {ip/servername}:{port}{_prefix}/{_index}
// prefix would include "/", like "/cgi-bin", "/index"
// index will be just name of the file like "index.html"
// so example of url
//		192.168.0.1:80/index/index.html
//		192.168.0.1:80/index/error.html
//
// if there is no prefix, index in request
//				>> leads to ultimate default html of the server
// if there is no index in request
//				>> leads to default html of the location  
struct location{

//data requested for general
		int				_httpMethods;
		std::string		_errorPage;
		std::string		_path;						 //where data from post/get(?) saved/called
		std::string		_redirect;
//data requested for index page
		bool							_dir_listing;// to show it in order(like alphabetic) automatically or not
		std::vector<std::string>		_index;

//data requested for cgi
		std::map<std::string, std::string>	_cgi;

};

typedef struct s_server
{
	std::string	 						port;
	std::string							workingDir;
	std::string 						host;
	std::string							server_name;
	std::string							root;				 //where webserver html/db/cgi/everything files are saved
	std::string 						default_error_page;
	long unsigned int					client_max_body_size;
	std::string							dir_request_default; //default file if the request is a directory
	std::map<std::string, location>		locations;			 //string will be the prefix, location is just a location
															//std::string		_prefix;	//like "/" "/cgi", "/index"

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
	std::string BodyBuffer;
	location	UsedRoute;
	std::string Body;
};

struct Response {
	std::string 						responseBuffer;
	std::map<std::string,std::string>	headerFields;
	statusCode							httpStatus;
	std::string 						body;
	bool								isReady;
};

#endif //WEBSERV_COMMONINCLUDES_HPP
