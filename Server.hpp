#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

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
#include "httpParser.hpp"

enum RequestType {
	INVALID = -1,
	GET,
	HEAD,
	POST,
	PUT,
	DELETE,
	CONNECT,
	OPTIONS,
	TRACE,
	PATCH,
};

enum sockoption{
	OFF,
	ON,
};

struct WebservConfigStruct {
	std::string	port;
	std::string	host;
	std::string	server_name;
	std::string	default_error_page;
	int			client_max_body_size;
	std::vector	<std::string> httpMethods;
	std::string	httpRedirection;
	std::vector	<std::string> path;
	std::map	<std::string, std::string> cgi_extension;
	bool		dir_listing;
	std::string	dir_request_default; //default file if the request is a directory
};

struct Request {
	std::string RequestBuffer;
	std::string Header;
	std::map<std::string,std::string> HeaderFields;
	RequestType ReqType;
	std::string RequestedPath;
	std::string Body;
};

/*
 * On construction, create one listening socket.
 * have a public container containing connected sockets.
 * have one public function that uses poll to check if new connections have arrived in queue.
 * if new connections are in queue, accept them and put them into container.
*/

class Server {
private:

	int socketOption;

	addrinfo hints;
	addrinfo *serverInfo;

	pollfd listening_socket;

	pollfd client;
	socklen_t client_address_length;

	char buffer[1000];

	std::vector<pollfd>Fds;
	std::map<int, Request>connectionMsgs;



public:
	Server(WebservConfigStruct settings);
	void CheckForConnections();
	~Server();
};


#endif //WEBSERV_SERVER_HPP

/*
 * struct s_Header{
 * 	//General header Fields
 * 	std::string CacheControl;
 * 	std::string Connection;
 * 	std::string Date;
 * 	std::string Pragma;
 * 	std::string Trailer;
 * 	std::string TransferEncoding;
 * 	std::string Upgrade;
 * 	std::string Via;
 * 	std::string Warning;
 * 	//Request header Fields
 * 	std::string Accept;
 * 	std::string AcceptCharset;
 * 	std::string AcceptEncoding;
 * 	std::string AcceptLanguage;
 * 	std::string Authorization;
 * 	std::string Expect;
 * 	std::string From;
 * 	std::string Host;
 * 	std::string IfMatch;
 * 	std::string IfModifiedSince;
 * 	std::string IfNoneMatch;
 * 	std::string IfRange;
 * 	std::string IfUnmodifiedSince;
 * 	std::string MaxForwards;
 * 	std::string ProxyAuthorization;
 * 	std::string Range;
 * 	std::string Referer;
 * 	std::string TE;
 * 	std::string UserAgent;
 * 	//Response Header Fields
 * 	std::string AcceptRanges;
 * 	std::string Age;
 * 	std::string ETag;
 * 	std::string Location;
 * 	std::string ProxyAuthenticate;
 * 	std::string RetryAfter;
 * 	std::string Server;
 * 	std::string Vary;
 * 	std::string WWWAuthenticate;
 * 	//Entity Header Fields
 * 	std::string Allow;
 * 	std::string ContentEncoding;
 * 	std::string ContentLanguage;
 * 	std::string ContentLength;
 * 	std::string ContentLocation;
 * 	std::string ContentMD5;
 * 	std::string ContentRange;
 * 	std::string ContentType;
 * 	std::string Expires;
 * 	std::string LastModified;
 * 	std::string extensionHeader;
 * };
 *
 */