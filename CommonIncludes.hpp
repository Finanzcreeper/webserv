#ifndef WEBSERV_COMMONINCLUDES_HPP
#define WEBSERV_COMMONINCLUDES_HPP

#include <vector>
#include <map>
#include"server/statusCodes.h"


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
class location{
	private:
		std::string		_prefix;//used in 
		int				_httpMethods;
		std::string		_index;//I don't get the concept of index yet, but so far I understand
							   //list of the files html/cgi/whatever files
							   //should be able to be approached from the location
		std::string		_errorPage;
								// default error page(html file), don't have to be included in _index above
								//if it is empty, copy the server's
		std::string		_path;//where data from post/get(?) saved/called
		bool			_dir_listing;// to show it in order(like alphabetic) automatically or not

};

class cgi: protected location{
	private:
		std::map<std::string, std::string>	cgi_extension;
};


typedef struct s_server
{
	std::string	 						port;
	std::string 						host;
	std::string							server_name;
	std::string							root;
	std::string 						default_error_page;
	long unsigned int					client_max_body_size;
	std::string							dir_request_default; //default file if the request is a directory
	std::vector<location*>				locations;   //shold be free each location when webserv program ended
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
