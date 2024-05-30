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
#include <exception>
#include "MethodExecutor.hpp"
#include "tools/statusCodes.h"

enum RequestIntegrity {
	OK,
	INVALID_HEADER,
	BODY_TOO_BIG,
	TIMED_OUT,
	UNSUPPORTED_REQUEST_TYPE
};

enum RequestType {
	NONE = -2,
	INVALID = -1,
	GET, // REQUIRED
	HEAD, // REQUIRED (not part of subject but mandatory for general purpose http server)
	POST, // REQUIRED
	PUT,
	DELETE, // REQUIRED
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
	long unsigned int			client_max_body_size;
	std::vector	<RequestType> httpMethods;
	std::string	httpRedirection;
	std::vector	<std::string> path;
	std::map	<std::string, std::string> cgi_extension;
	bool		dir_listing;
	std::string	dir_request_default; //default file if the request is a directory
};

struct Request {
	std::string RequestBuffer;
	std::string HeaderBuffer;
	std::map<std::string,std::string> HeaderFields;
	RequestType ReqType;
	RequestIntegrity Integrity;
	std::string RequestedPath;
	std::string BodyBuffer;
	std::string Body;
};

struct Response {
	std::string 	responseBuffer;
	std::string 	headerBuffer;
	statusCode		statusCode;
	std::string 	body;
};

/*
 *    |
 * GET / HDYGDEGD
 *      |
 * On construction, create one listening socket.
 * have a public container containing connected sockets.
 * have one public function that uses poll to check if new connections have arrived in queue.
 * if new connections are in queue, accept them and put them into container.
*/


class Server {
private:

	const WebservConfigStruct settings;

	int socketOption;

	addrinfo hints;
	addrinfo *serverInfo;

	pollfd listening_socket;

	pollfd client;
	//socklen_t client_address_length;

	char buffer[1000];

	std::vector<pollfd>Fds;
	std::map<int, Request>connectionMsgs;
	std::map<int, Response>answerMsgs;

	void responder(std::map <int, Response>::iterator& response);

public:
	Server(WebservConfigStruct sett);
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
 * 	//Response HeaderBuffer Fields
 * 	std::string AcceptRanges;
 * 	std::string Age;
 * 	std::string ETag;
 * 	std::string Location;
 * 	std::string ProxyAuthenticate;
 * 	std::string RetryAfter;
 * 	std::string Server;
 * 	std::string Vary;
 * 	std::string WWWAuthenticate;
 * 	//Entity HeaderBuffer Fields
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