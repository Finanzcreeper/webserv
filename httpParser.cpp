#include "httpParser.hpp"
#include <sstream>

httpParser::httpParser(std::map<int, Request>::iterator& pair, const WebservConfigStruct sett): req(pair), settings(sett) {
	beheader(req->second);
	if (req->second.HeaderBuffer.empty() == true) {
		return;
	}
	if (req->second.BodyBuffer.empty() == true) {
		return;
	}
	handleBody(req->second);
}

void httpParser::beheader(Request& request) {
	size_t neck = request.RequestBuffer.find("\r\n\r\n");
	if (neck == request.RequestBuffer.npos) {
		std::cerr << "invalid/incomplete header: \n" << std::endl;
		return;
	} else if (request.HeaderBuffer.empty() == true){
		request.HeaderBuffer = request.RequestBuffer.substr(0, neck + 4);
		request.RequestBuffer.erase(0,neck+4);
		GetRequestType(request);
		GetRequestedPath(request);
		decapitalizeHeaderFields(request.HeaderBuffer);
		extractHeaderFields(request);
	}
	request.BodyBuffer.append(request.RequestBuffer.substr(0, request.RequestBuffer.size()));
	request.RequestBuffer.erase(0,request.RequestBuffer.size());
	if (request.Body.size() > settings.client_max_body_size) {
		this->req->second.Integrity = BODY_TOO_BIG;
	}
	//std::cout << request.HeaderBuffer << std::endl;
}

void httpParser::GetRequestType(Request& request) {
	if (request.HeaderBuffer.find("GET") == 0) {
		request.ReqType = GET;
	} else if (request.HeaderBuffer.find("HEAD") == 0) {
		request.ReqType = HEAD;
	}else if (request.HeaderBuffer.find("POST") == 0) {
		request.ReqType = POST;
	} else if (request.HeaderBuffer.find("PUT") == 0) {
		request.ReqType = PUT;
	} else if (request.HeaderBuffer.find("DELETE") == 0) {
		request.ReqType = DELETE;
	} else if (request.HeaderBuffer.find("CONNECT") == 0) {
		request.ReqType = CONNECT;
	} else if (request.HeaderBuffer.find("OPTIONS") == 0) {
		request.ReqType = OPTIONS;
	} else if (request.HeaderBuffer.find("TRACE") == 0) {
		request.ReqType = TRACE;
	} else if (request.HeaderBuffer.find("PATCH") == 0) {
		request.ReqType = PATCH;
	} else {
		request.ReqType = INVALID;
	}
}

void httpParser::GetRequestedPath(Request& request) {
	request.RequestedPath = request.HeaderBuffer.substr(
		request.HeaderBuffer.find(' ') + 1,
		request.HeaderBuffer.find(' ', request.HeaderBuffer.find(' ') + 1) - request.HeaderBuffer.find(' '));
}

void httpParser::decapitalizeHeaderFields(std::string& Header) {
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

void httpParser::extractHeaderFields(Request& req) {
	std::vector<std::string> SearchedHeaderFields;
	//Add HeaderBuffer fields to extract here
	SearchedHeaderFields.push_back("connection");
	SearchedHeaderFields.push_back("transfer-encoding");
	SearchedHeaderFields.push_back("user-agent");

	long unsigned int i = 0;
	while (i < SearchedHeaderFields.size()) {
		if (req.HeaderBuffer.find(SearchedHeaderFields[i]) != req.HeaderBuffer.npos) {
			req.HeaderFields.insert(std::make_pair(SearchedHeaderFields[i],req.HeaderBuffer.substr(req.HeaderBuffer.find(SearchedHeaderFields[i]) + SearchedHeaderFields[i].size() + 2, req.HeaderBuffer.find('\n', req.HeaderBuffer.find(SearchedHeaderFields[i])) - (req.HeaderBuffer.find(SearchedHeaderFields[i]) + SearchedHeaderFields[i].size() + 3))));
		}
		++i;
	}
}

void httpParser::handleBody(Request& req) {
	std::map<std::string ,std::string>::iterator trenc;
	int ChunkSize = 0;
	trenc = req.HeaderFields.find("transfer-encoding");

	if (trenc == req.HeaderFields.end()) {
		return;
	}
	if (trenc->second == "chunked") {
		while (req.BodyBuffer.empty() == false) {
			std::string ChunkHexSize = req.BodyBuffer.substr(
				0, req.BodyBuffer.find_first_of("\r\n"));
			req.BodyBuffer.erase(0,req.BodyBuffer.find("\r\n",0) + 2);
			std::istringstream HexStream(ChunkHexSize);
			HexStream >> std::hex >> ChunkSize;
			req.Body.append(req.BodyBuffer.substr(0,ChunkSize));
			req.BodyBuffer.erase(0,ChunkSize + 2);
		}
	}
	else {
		req.Body.append(req.BodyBuffer);
		req.BodyBuffer.erase();
	}
}

