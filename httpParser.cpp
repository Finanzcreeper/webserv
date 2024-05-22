#include "httpParser.hpp"

void httpParser(std::pair<int, Request> pair) {
	beheader(pair.second);
	GetRequestType(pair.second);
	GetRequestedPath(pair.second);
	decapitalizeHeaderFields(pair.second.Header);
	extractHeaderFields(pair.second);


	//output only
	std::map<std::string , std::string>::iterator out;
	out = pair.second.HeaderFields.begin();
	std::cout << "extracted " << pair.second.HeaderFields.size() << " fields" << std::endl;
	while (out != pair.second.HeaderFields.end()) {
		std::cout << out->first << ": " << out->second << std::endl;
		++out;
	}
}

void beheader(Request& request) {
	size_t neck = request.RequestBuffer.find("\r\n\r\n");
	if (neck == request.RequestBuffer.npos) {
		std::cerr << "invalid/incomplete header: \n"<< /*request->second.RequestBuffer << */std::endl;
		return;
		//throw invalid header exception
	} else {
		request.Header = request.RequestBuffer.substr(0, neck + 4);
	}
	if (request.RequestBuffer.rfind("\r\n\r\n") <= neck) {
		std::cerr << "no body found" << std::endl;
	} else {
		request.Body = request.RequestBuffer.substr(neck + 4, request.RequestBuffer.size());
	}
	//std::cout << request.Header << std::endl;
}

void GetRequestType(Request& request) {
	if (request.Header.find("GET") == 0) {
		request.ReqType = GET;
	} else if (request.Header.find("HEAD") == 0) {
		request.ReqType = HEAD;
	}else if (request.Header.find("POST") == 0) {
		request.ReqType = POST;
	} else if (request.Header.find("PUT") == 0) {
		request.ReqType = PUT;
	} else if (request.Header.find("DELETE") == 0) {
		request.ReqType = DELETE;
	} else if (request.Header.find("CONNECT") == 0) {
		request.ReqType = CONNECT;
	} else if (request.Header.find("OPTIONS") == 0) {
		request.ReqType = OPTIONS;
	} else if (request.Header.find("TRACE") == 0) {
		request.ReqType = TRACE;
	} else if (request.Header.find("PATCH") == 0) {
		request.ReqType = PATCH;
	} else {
		request.ReqType = INVALID;
	}
}

void GetRequestedPath(Request& request) {
	request.RequestedPath = request.Header.substr(
		request.Header.find(' ') + 1,
		request.Header.find("\r\n") - request.Header.find(' ') - 9);
	std::cout << request.RequestedPath << std::endl;
}

void decapitalizeHeaderFields(std::string& Header) {
	int i = 0;
	while(Header[i] != '\n') {
		i++;
	}
	while (Header[i]) {
		Header[i] = tolower(Header[i]);
		i++;
		if ( Header[i] == ':') {
			while (Header[i] != '\n') {
				++i;
			}
		}
	}
}

void extractHeaderFields(Request& req) {
	std::vector<std::string> SearchedHeaderFields;
	//Add Header fields to extract here
	SearchedHeaderFields.push_back("connection");
	SearchedHeaderFields.push_back("transferencoding");
	SearchedHeaderFields.push_back("user-agent");

	long unsigned int i = 0;
	while (i < SearchedHeaderFields.size()) {
		if (req.Header.find(SearchedHeaderFields[i]) != req.Header.npos) {
			req.HeaderFields.insert(std::make_pair(SearchedHeaderFields[i],req.Header.substr(req.Header.find(SearchedHeaderFields[i])+SearchedHeaderFields[i].size()+2, req.Header.find('\n',req.Header.find(SearchedHeaderFields[i])) - (req.Header.find(SearchedHeaderFields[i])+SearchedHeaderFields[i].size()+2))));
		}
		++i;
	}
}

