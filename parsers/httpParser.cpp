#include "httpParser.hpp"
#include <sstream>

httpParser::httpParser(std::map<int, Request>::iterator& pair, const t_server& sett): req(pair), settings(sett) {
	size_t endOfBlock = req->second.RequestBuffer.find("\r\n\r\n");
	if ( endOfBlock == std::string::npos) {
		return;
	}
	if (req->second.HeaderBuffer.empty() == true) {
		handleHeader(req->second, endOfBlock);
	}
	if (req->second.Body.empty() == true) {
		handleBody(req->second, endOfBlock);
	}
	if (req->second.RequestBuffer.empty() == false) {
		req->second.RequestIntegrity = BAD_REQUEST;
		throw std::runtime_error("Content after Body recieved!");
	}
}

void httpParser::handleHeader(Request &request, size_t endOfBlock) {
	request.HeaderBuffer = request.RequestBuffer.substr(0, endOfBlock + 4);
	request.RequestBuffer.erase(0,endOfBlock + 4);
	GetRequestType(request);
	GetRequestedPath(request);
	decapitalizeHeaderFields(request.HeaderBuffer);
	extractHeaderFields(request);
}

void httpParser::GetRequestType(Request& request) {
	std::string firstLine = request.HeaderBuffer.substr(0,request.HeaderBuffer.find('\r'));
	if (firstLine.find("GET") == 0) {
		request.ReqType = GET;
	} else if (firstLine.find("HEAD") == 0) {
		request.ReqType = HEAD;
	}else if (firstLine.find("POST") == 0) {
		request.ReqType = POST;
	} else if (firstLine.find("PUT") == 0) {
		request.ReqType = PUT;
	} else if (firstLine.find("DELETE") == 0) {
		request.ReqType = DELETE;
	} else if (firstLine.find("CONNECT") == 0) {
		request.ReqType = CONNECT;
	} else if (firstLine.find("OPTIONS") == 0) {
		request.ReqType = OPTIONS;
	} else if (firstLine.find("TRACE") == 0) {
		request.ReqType = TRACE;
	} else if (firstLine.find("PATCH") == 0) {
		request.ReqType = PATCH;
	} else {
		request.ReqType = INVALID;
	}
	if ((request.ReqType & settings.httpMethods) == 0) {
		request.RequestIntegrity = METHOD_NOT_ALLOWED;
	}
	if (request.RequestIntegrity != OK_HTTP) {
		throw std::runtime_error("Unsupported Request type recieved!");
	}
}

void httpParser::GetRequestedPath(Request& request) {
	int startOfPath = request.HeaderBuffer.find(' ') + 1;
	request.RequestedPath = request.HeaderBuffer.substr(startOfPath, request.HeaderBuffer.find(' ', startOfPath) - startOfPath);
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


void httpParser::handleBody(Request &request, size_t endOfBlock) {
	std::map<std::string ,std::string>::iterator TransferCoding;
	int ChunkSize = 0;
	TransferCoding = request.HeaderFields.find("transfer-encoding");
	if (TransferCoding != request.HeaderFields.end() && TransferCoding->second == "chunked") {
		while (request.RequestBuffer.empty() == false) {
			std::string ChunkHexSize = request.RequestBuffer.substr(
				0, request.RequestBuffer.find_first_of("\r\n"));
			request.RequestBuffer.erase(0, request.RequestBuffer.find("\r\n", 0) + 2);
			std::istringstream HexStream(ChunkHexSize);
			HexStream >> std::hex >> ChunkSize;
			request.Body.append(request.RequestBuffer.substr(0, ChunkSize));
			request.RequestBuffer.erase(0, ChunkSize + 2);
		}
	} else {
		request.Body.append(request.RequestBuffer.substr(0,endOfBlock + 4));
		request.RequestBuffer.erase(0, endOfBlock + 4);
	}
	if (request.Body.size() > settings.client_max_body_size) {
		this->req->second.RequestIntegrity = PAYLOAD_TO_LARGE;
		throw std::runtime_error("Http request has an oversized Body!");
	}
}

