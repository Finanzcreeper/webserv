#include "httpParser.hpp"
#include <sstream>

void httpParser(std::map<int, connection>::iterator& req) {
	size_t endOfBlock = req->second.r.RequestBuffer.find("\r\n\r\n");
	if ( endOfBlock == std::string::npos) {
		return;
	}
	if (req->second.r.HeaderBuffer.empty() == true) {
		handleHeader(req->second.r, endOfBlock);
	}
	endOfBlock = req->second.r.RequestBuffer.find("\r\n\r\n");
	if ( endOfBlock == std::string::npos) {
		return;
	}
	if (req->second.r.Body.empty() == true) {
		handleBody(req->second.r, endOfBlock);
	}
	req->second.r.requestCompletlyRecieved = true;
	if (req->second.r.RequestBuffer.empty() == false) {
		req->second.r.RequestIntegrity = BAD_REQUEST;
		return;
	}
}

void handleHeader(Request &request, size_t endOfBlock) {
	request.HeaderBuffer = request.RequestBuffer.substr(0, endOfBlock + 4);
	request.RequestBuffer.erase(0,endOfBlock + 4);
	GetRequestType(request);
	GetRequestedPath(request);
	decapitalizeHeaderFields(request.HeaderBuffer);
	extractHeaderFields(request);
	if (request.HeaderFields.find("content-length")->second.empty() == true && request.HeaderFields.find("transfer-encoding")->second.empty() == true) {
		request.requestCompletlyRecieved = true;
	}
}

void GetRequestType(Request& request) {
	std::string firstLine = request.HeaderBuffer.substr(0,request.HeaderBuffer.find('\r'));
	if (firstLine.find("GET ") == 0) {
		request.ReqType = GET;
	} else if (firstLine.find("HEAD ") == 0) {
		request.ReqType = HEAD;
	} else if (firstLine.find("POST ") == 0) {
		request.ReqType = POST;
	} else if (firstLine.find("PUT ") == 0) {
		request.ReqType = PUT;
	} else if (firstLine.find("DELETE ") == 0) {
		request.ReqType = DELETE;
	} else if (firstLine.find("CONNECT ") == 0) {
		request.ReqType = CONNECT;
	} else if (firstLine.find("OPTIONS ") == 0) {
		request.ReqType = OPTIONS;
	} else if (firstLine.find("TRACE ") == 0) {
		request.ReqType = TRACE;
	} else if (firstLine.find("PATCH ") == 0) {
		request.ReqType = PATCH;
	} else {
		request.ReqType = INVALID;
	}
}

void GetRequestedPath(Request& request) {
	size_t startOfPath = request.HeaderBuffer.find(' ') + 1;
	request.RequestedPath = request.HeaderBuffer.substr(startOfPath, request.HeaderBuffer.find(' ', startOfPath) - startOfPath);
}

void decapitalizeHeaderFields(std::string& Header) {
	int i = 0;
	while(Header[i] != '\n') {
		i++;
	}
	while (Header[i]) {
		Header[i] = static_cast<char>(tolower(Header[i]));
		i++;
		if ( Header[i] == ':') {
			while (Header[i] != '\n') {
				++i;
			}
		}
	}
}

void extractHeaderFields(Request& request) {
	std::vector<std::string> SearchedHeaderFields;
	//Add HeaderBuffer fields to extract here
	SearchedHeaderFields.push_back("connection");
	SearchedHeaderFields.push_back("transfer-encoding");
	SearchedHeaderFields.push_back("user-agent");
	SearchedHeaderFields.push_back("content-type");
	SearchedHeaderFields.push_back("content-length");


	long unsigned int i = 0;
	while (i < SearchedHeaderFields.size()) {
		if (request.HeaderBuffer.find(SearchedHeaderFields[i]) != std::string::npos) {
			request.HeaderFields.insert(std::make_pair(SearchedHeaderFields[i],request.HeaderBuffer.substr(request.HeaderBuffer.find(SearchedHeaderFields[i]) + SearchedHeaderFields[i].size() + 2, request.HeaderBuffer.find('\n', request.HeaderBuffer.find(SearchedHeaderFields[i])) - (request.HeaderBuffer.find(SearchedHeaderFields[i]) + SearchedHeaderFields[i].size() + 3))));
		}
		++i;
	}
}


void handleBody(Request &request, size_t endOfBlock) {
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
}

