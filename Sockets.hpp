#ifndef WEBSERV_SOCKETS_HPP
#define WEBSERV_SOCKETS_HPP

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

enum sockoption{
	OFF,
	ON,
};

struct s_Header{
	//General header Fields
	std::string CacheControl;
	std::string Connection;
	std::string Date;
	std::string Pragma;
	std::string Trailer;
	std::string TransferEncoding;
	std::string Upgrade;
	std::string Via;
	std::string Warning;
	//Request header Fields
	std::string Accept;
	std::string AcceptCharset;
	std::string AcceptEncoding;
	std::string AcceptLanguage;
	std::string Authorization;
	std::string Expect;
	std::string From;
	std::string Host;
	std::string IfMatch;
	std::string IfModifiedSince;
	std::string IfNoneMatch;
	std::string IfRange;
	std::string IfUnmodifiedSince;
	std::string MaxForwards;
	std::string ProxyAuthorization;
	std::string Range;
	std::string Referer;
	std::string TE;
	std::string UserAgent;
	//Response Header Fields
	std::string AcceptRanges;
	std::string Age;
	std::string ETag;
	std::string Location;
	std::string ProxyAuthenticate;
	std::string RetryAfter;
	std::string Server;
	std::string Vary;
	std::string WWWAuthenticate;
	//Entity Header Fields
	std::string Allow;
	std::string ContentEncoding;
	std::string ContentLanguage;
	std::string ContentLength;
	std::string ContentLocation;
	std::string ContentMD5;
	std::string ContentRange;
	std::string ContentType;
	std::string Expires;
	std::string LastModified;
	std::string extensionHeader;
};

struct Request {
	std::string RequestBuffer;
	std::string Header;
	s_Header HeaderFields_s;
	std::map<std::string,std::string> HeaderFields;
	std::string Body;
};

/*
 * On construction, create one listening socket.
 * have a public container containing connected sockets.
 * have one public function that uses poll to check if new connections have arrived in queue.
 * if new connections are in queue, accept them and put them into container.
*/

class Sockets {
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

	void beheader(std::map<int, Request>::iterator mt);
	void decapitalizeHeaderFields(std::string& Header);
	void extractHeaderFields(Request req);

public:
	Sockets();
	void CheckForConnections();
	~Sockets();
};


#endif //WEBSERV_SOCKETS_HPP
