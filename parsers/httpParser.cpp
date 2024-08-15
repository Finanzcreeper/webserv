#include "httpParser.hpp"
#include <sstream>

void httpParser(std::map<int, connection>::iterator& req) {
	size_t endOfBlock;
	if (req->second.r.HeaderBuffer.empty() == true) {
		endOfBlock = findEndOfBlock(req->second.r.RequestBuffer);
		if (endOfBlock == std::string::npos) {
			return;
		}
		handleHeader(req->second.r, endOfBlock);
	}
	if (hasBody(req->second.r) == false) {
		req->second.r.requestCompletlyRecieved = true;
		return;
	} else if (req->second.r.Body.empty() == true) {
		handleBody(req->second.r);
	}
	
	/*if (req->second.r.RequestBuffer.empty() == false) {
		req->second.r.RequestIntegrity = BAD_REQUEST;
		return;
	}*/
}

size_t findEndOfBlock(std::string buffer) {
	size_t endOfBlock = buffer.find("\r\n\r\n");
	if (endOfBlock == std::string::npos) {
		endOfBlock = buffer.find("\n\n");
	}
	return (endOfBlock);
}

bool hasBody(Request& request) {
	if (request.HeaderFields.find("content-length") == request.HeaderFields.end() && request.HeaderFields.find("transfer-encoding") == request.HeaderFields.end()) {
		return (false);
	}
	return (true);
}

void handleHeader(Request &request, size_t endOfBlock) {
	request.HeaderBuffer = request.RequestBuffer.substr(0, endOfBlock + 4);
	request.RequestBuffer.erase(0,endOfBlock + 4);
	GetRequestType(request);
	GetRequestedPath(request);
	decapitalizeHeaderFields(request.HeaderBuffer);
	extractHeaderFields(request);
	checkMultipartDelimiter(request);
	if (request.HeaderFields.find("content-length") == request.HeaderFields.end() && request.HeaderFields.find("transfer-encoding") == request.HeaderFields.end()) {
		request.requestCompletlyRecieved = true;
	}
}

void GetRequestType(Request& request) {
	std::string firstLine = request.HeaderBuffer.substr(0,request.HeaderBuffer.find('\n'));
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
	if (request.HeaderBuffer.find(' ') == std::string::npos) {
		request.RequestedPath.clear();
		return;
	}
	size_t startOfPath = request.HeaderBuffer.find(' ') + 1;
	request.RequestedPath = request.HeaderBuffer.substr(startOfPath, request.HeaderBuffer.find(' ', startOfPath) - startOfPath);
}

void decapitalizeHeaderFields(std::string& Header) {
	size_t i = 0;
	while(Header[i] != '\n' && i < Header.size()) {
		i++;
	}
	while (i < Header.size()) {
		Header[i] = static_cast<char>(tolower(Header[i]));
		i++;
		if ( Header[i] == ':') {
			while (Header[i] != '\n' && i < Header.size()) {
				++i;
			}
		}
	}
}

void checkMultipartDelimiter(Request& request) {
	std::map<std::string,std::string>::iterator it;
	it = request.HeaderFields.find("content-type");
	if (it == request.HeaderFields.end()) {
		return;
	}
	if( it->second.find("multipart/form-data; boundary=") == std::string::npos) {
		request.RequestIntegrity = BAD_REQUEST;
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


void handleBody(Request &request) {
	std::map<std::string ,std::string>::iterator TransferCoding;
	std::map<std::string ,std::string>::iterator ContentLenght;
	int ChunkSize = 0;
	TransferCoding = request.HeaderFields.find("transfer-encoding");
	ContentLenght = request.HeaderFields.find("content-length");
	if (TransferCoding != request.HeaderFields.end() && TransferCoding->second == "chunked") {
		if (request.RequestBuffer.find("0\r\n\r\n") == std::string::npos){
			request.requestCompletlyRecieved = false;
			return;
		}
		while (request.RequestBuffer.empty() == false) {
			std::string ChunkHexSize = request.RequestBuffer.substr(
				0, request.RequestBuffer.find_first_of("\r\n"));
			request.RequestBuffer.erase(0, request.RequestBuffer.find("\r\n", 0) + 2);
			std::istringstream HexStream(ChunkHexSize);
			HexStream >> std::hex >> ChunkSize;
			request.Body.append(request.RequestBuffer.substr(0, ChunkSize));
			request.RequestBuffer.erase(0, ChunkSize + 2);
		}
	} else if(ContentLenght != request.HeaderFields.end()) {
		std::istringstream iss(ContentLenght->second);
		size_t contentLength;
		iss >> contentLength;
		if (request.RequestBuffer.size() < contentLength) {
			request.requestCompletlyRecieved = false;
			return;
		}
		request.Body.append(request.RequestBuffer.substr(0,contentLength));
		request.RequestBuffer.clear();
		request.requestCompletlyRecieved = true;
	} else {
		request.RequestIntegrity = LENGTH_REQUIRED;
	}
}
