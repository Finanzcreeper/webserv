#include "httpInterpreter.hpp"

void interpretRequest(Request& request, const t_server& settings) {
	findRoute(request,settings);
	if (request.RequestIntegrity != OK_HTTP) {
		return;
	}
	checkIfMethodIsAllowedOnRoute(request);
	if (request.RequestIntegrity != OK_HTTP) {
		return;
	}
	checkBodySize(request, settings);
	if (request.RequestIntegrity != OK_HTTP) {
		return;
	}
	redirectionChecker(request);
	if (request.RequestIntegrity != OK_HTTP) {
		return;
	}
}

#include <iostream>
#include <algorithm>
#include <stdlib.h>
void handleMultipart (Request& request) {
	std::string delimiter;
	std::string BodyBuffer;

	std::map<std::string,std::string>::iterator it;
	it = request.HeaderFields.find("content-type");
	if (it == request.HeaderFields.end()) {
		return;
	}

	delimiter = MultipartDelimiterValidation(request);
	if (request.RequestIntegrity != OK_HTTP) {
		return;
	}

	std::map<std::string,std::string>::iterator lt = request.HeaderFields.find("content-length");
	if (lt == request.HeaderFields.end()) {
		request.RequestIntegrity = LENGTH_REQUIRED;
		return;
	}

	unsigned long length = std::atoi(lt->second.c_str());
	if (length != request.Body.size()) {
///*			*/std::cout << request.Body.size() << std::endl;
		request.RequestIntegrity = BAD_REQUEST;
		return;
	}

	Multipart mp;
	std::string endDelimiter = delimiter + "--";
	request.Body.erase(0, request.Body.find(delimiter) + delimiter.size());
///*			*/std::cout << "\033[1;34mINIT: \033[0m"  << request.Body << std::endl;
///*			*/std::cout << "\033[1;33m=====================================================\033[0m" << std::endl;
	while (request.Body.find(endDelimiter) != 0) {
		if (request.Body.find(delimiter) == 0) {
			request.Body.erase(0, request.Body.find(delimiter) + delimiter.size());
///*			*/std::cout << "\033[1;34mDELIM REMOVED: \033[0m" << request.Body << std::endl;
		}
		if (request.Body.find("\r\n\r\n") == 0) {
			//found multipart body (because double crlf)
			mp.Body = request.Body.substr(4,request.Body.find(delimiter) - 4);
///*			*/std::cout << request.Body.find(delimiter) << std::endl;
			request.bodyParts.push_back(mp);
			mp.MultipartHeaderFields.clear();
			request.Body.erase(0, request.Body.find(delimiter));
///*			*/std::cout << "\033[1;34mBODY REMOVED: \033[0m" << request.Body << std::endl;
		} else if (request.Body.find("\r\n") == 0) {
			//found multipart header (because single crlf)
			request.Body.erase(0,2);
			std::string Head = request.Body.substr(0,request.Body.find(":"));
			std::string Data = request.Body.substr(request.Body.find(":") + 2,(request.Body.find("\r\n") - (request.Body.find(":") + 2)));
			mp.MultipartHeaderFields.insert(std::make_pair(Head ,Data));
			request.Body.erase(0,request.Body.find("\r\n"));
///*			*/std::cout << "\033[1;34mREMOVED HEADER LINE: \033[0m" << request.Body << std::endl;
		} else {
			request.RequestIntegrity = BAD_REQUEST;
			break;
		}
///*			*/std::cout << "\033[1;33m=====================================================\033[0m" << std::endl;
	}
	request.Body.clear();
}

std::string MultipartDelimiterValidation(Request& request) {
	std::string delimiter;

	std::map<std::string,std::string>::iterator it;
	it = request.HeaderFields.find("content-type");
	delimiter = it->second.substr(it->second.find("=") + 1, it->second.size());
	if (delimiter.find("\"") == 0 && delimiter.find_last_of("\"") == delimiter.size() - 1) {
		delimiter = delimiter.substr(1,delimiter.size() - 2);
	}
	if (delimiter.size() <= 0 || delimiter.size() > 70) {
		request.RequestIntegrity = BAD_REQUEST;
		delimiter.clear();
		return (delimiter);
	}
	if (IsAllowedDelimChar(delimiter) == false) {
		request.RequestIntegrity = BAD_REQUEST;
		delimiter.clear();
		return (delimiter);
	}
	delimiter.insert(0,"\r\n--");
	return (delimiter);
}

bool IsAllowedDelimChar(std::string delim) {
	std::string::iterator it;
	it = delim.begin();
	while (it != delim.end()) {
		if (*it >= '\'' && *it <= ')') {
			++it;
		} else if (*it >= '+' && *it <= ':') {
			++it;
		} else if (*it == '=') {
			++it;
		} else if (*it == '?') {
			++it;
		} else if (*it >= 'A' && *it <= 'Z') {
			++it;
		} else if (*it == '_') {
			++it;
		} else if (*it >= 'a' && *it <= 'z') {
			++it;
		} else {
			return (false);
		}
	}
	return (true);
}

void findRoute(Request& request,  const t_server& settings) {
	std::map<std::string, location>::const_iterator RouteIterator;
	std::vector<std::string> possiblePaths;
	std::vector<std::string>::iterator reqPathPartsIt;
	size_t back = 0;
	bool routeSet = false;

	while (back < request.RequestedPath.size()) {
		back = request.RequestedPath.find('/',back);
		if (back != std::string::npos) {
			possiblePaths.push_back(request.RequestedPath.substr(0, back + 1));
			++back;
		} else {
			possiblePaths.push_back(request.RequestedPath.substr(0, request.RequestedPath.size()));
			break;
		}
	}
	reqPathPartsIt = possiblePaths.begin();
	while (reqPathPartsIt != possiblePaths.end() && reqPathPartsIt->empty() == false){
		RouteIterator = settings.locations.find(*reqPathPartsIt);
		if (RouteIterator != settings.locations.end()) {
 			request.UsedRoute = RouteIterator->second;
			routeSet = true;
		}
		++reqPathPartsIt;
	}
	if (routeSet == false) {
		request.RequestIntegrity = NOT_FOUND;
	} else {
		request.RoutedPath = request.RequestedPath;
		request.RoutedPath.replace(0,request.UsedRoute.locationName.size(),request.UsedRoute.root);
	}
}

void checkIfMethodIsAllowedOnRoute(Request& request) {
	if ((request.ReqType & request.UsedRoute.httpMethods) == 0) {
		request.RequestIntegrity = METHOD_NOT_ALLOWED;
	}
}

void checkBodySize(Request& request, const t_server& settings) {
	if (request.Body.size() > settings.clientMaxBodySize) {
		request.RequestIntegrity = PAYLOAD_TO_LARGE;
	}
}

void redirectionChecker(Request& request) {
	if (request.UsedRoute.redirect.empty() == false) {
		request.RequestIntegrity = MOVED_PERMANENTLY;
	}
}

void checkContentType(Request& request) {
	std::map<std::string,std::string>::iterator headerField;
	headerField = request.HeaderFields.find("content-type");
	if(headerField == request.HeaderFields.end()) {
		return;
	}

	std::vector<std::string> allowedContentTypes;
	std::vector<std::string> allowedContentSubtypes;

	allowedContentTypes.push_back("text/");
	allowedContentSubtypes.push_back("plain");

	std::string foundContentType;
	std::string foundContentSubtype;

	foundContentType = headerField->second.substr(0,headerField->second.find('/') + 1);
	foundContentSubtype = headerField->second.substr(headerField->second.find('/') + 1,headerField->second.size());
	bool contentTypeFound = false;
	bool contentSubtypeFound = false;

	std::vector<std::string>::iterator allowedContentTypeIterator = allowedContentTypes.begin();
	while (allowedContentTypeIterator != allowedContentTypes.end()) {
		if  (*allowedContentTypeIterator == foundContentType) {
			contentTypeFound = true;
			break;
		}
		++allowedContentTypeIterator;
	}
	if  (contentTypeFound == false) {
		request.RequestIntegrity = UNSUPPORTED_MEDIA_TYPE;
	} else {
		std::vector<std::string>::iterator allowedContentSubtypeIterator = allowedContentSubtypes.begin();
		while (allowedContentSubtypeIterator != allowedContentSubtypes.end()) {
			if (*allowedContentSubtypeIterator == foundContentSubtype) {
				contentSubtypeFound = true;
				break;
			}
			++allowedContentSubtypeIterator;
		}
	}
	if (contentSubtypeFound == false) {
		request.RequestIntegrity = UNSUPPORTED_MEDIA_TYPE;
	}
}
