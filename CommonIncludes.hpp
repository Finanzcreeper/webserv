#ifndef WEBSERV_COMMONINCLUDES_HPP
#define WEBSERV_COMMONINCLUDES_HPP

#include <vector>
#include <map>
#include"server/statusCodes.h"

struct location{

//data requested for general
		int				_httpMethods;
		std::string		_redirect;
//data requested for index page
		bool			_dir_listing;// to show it in order(like alphabetic) automatically or not
		std::string		_index;

//data requested for cgi
		std::map<std::string, std::string>	_cgi;
		std::string							_root;

		std::string				 			locationName;

	//	std::string		_path;						 //where data from post/get(?) saved/called
};

typedef struct s_server
{
	std::string	 						port;
	std::string 						host;
	std::string							server_name;
	std::map<int, std::string>			error_pages;
	long unsigned int					client_max_body_size;
	int									timeoutTime;
	int									timeoutReads;
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
