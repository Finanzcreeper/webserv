#include "httpInterpreter.hpp"
#include <iostream>
#include <algorithm>
#include <stdlib.h>

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
	checkContentType(request);
	if (request.RequestIntegrity != OK_HTTP) {
		return;
	}
	handleMultipart(request);
}

void handleMultipart (Request& request) {
	std::string delimiter;
	std::string BodyBuffer;

	request.bodyParts.clear();
	std::map<std::string,std::string>::iterator it;
	it = request.HeaderFields.find("content-type");
	if (it == request.HeaderFields.end()) {
		return;
	} else if (it->second.find("multipart") != 0) {
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
		request.RequestIntegrity = BAD_REQUEST;
		return;
	}

	Multipart mp;
	std::string endDelimiter = delimiter + "--";

	if (request.Body.find(endDelimiter) == std::string::npos) {
		request.RequestIntegrity = BAD_REQUEST;
		return;
	}
	request.Body.erase(0, request.Body.find(delimiter) + delimiter.size());
	while (request.Body.find(endDelimiter) != 0) {
		if (request.Body.find(delimiter) == 0) {
			request.Body.erase(0, request.Body.find(delimiter) + delimiter.size());
		}
		if (request.Body.find("\r\n\r\n") == 0) {
			//found multipart body (because double crlf)
			mp.Body = request.Body.substr(4,request.Body.find(delimiter) - 4);
			request.bodyParts.push_back(mp);
			mp.MultipartHeaderFields.clear();
			request.Body.erase(0, request.Body.find(delimiter));
		} else if (request.Body.find("\r\n") == 0) {
			//found multipart header (because single crlf)
			request.Body.erase(0,2);
			std::string Head = request.Body.substr(0,request.Body.find(":"));
			std::string Data = request.Body.substr(request.Body.find(":") + 2,(request.Body.find("\r\n") - (request.Body.find(":") + 2)));
			mp.MultipartHeaderFields.insert(std::make_pair(Head ,Data));
			request.Body.erase(0,request.Body.find("\r\n"));
		} else {
			request.RequestIntegrity = BAD_REQUEST;
			break;
		}
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

	std::map<std::string,std::vector<std::string> > allowedContentTypeMap;
	std::map<std::string,std::vector<std::string> >::iterator allowedContentTypeIterator;

	std::vector<std::string> text;
	text.push_back("plain");

	std::vector<std::string> multipart;
	multipart.push_back("form-data");

	std::vector<std::string> application;
	application.push_back("x-www-form-urlencoded");

	std::vector<std::string>::iterator ContentSubtypeIterator;

	allowedContentTypeMap.insert((std::make_pair("application/",application)));
	allowedContentTypeMap.insert(std::make_pair("text/", text));
	allowedContentTypeMap.insert((std::make_pair("multipart/", multipart)));

	std::string foundContentType;
	std::string foundContentSubtype;

	foundContentType = headerField->second.substr(0,headerField->second.find('/') + 1);
	foundContentSubtype = headerField->second.substr(headerField->second.find('/') + 1,std::min(headerField->second.size(),headerField->second.find(";") - headerField->second.find('/') -1 ));
	allowedContentTypeIterator = allowedContentTypeMap.find(foundContentType);
	if (allowedContentTypeIterator == allowedContentTypeMap.end()) {
		request.RequestIntegrity = UNSUPPORTED_MEDIA_TYPE;
		return;
	}
	ContentSubtypeIterator = std::find(allowedContentTypeIterator->second.begin(), allowedContentTypeIterator->second.end(), foundContentSubtype);
	if (ContentSubtypeIterator == allowedContentTypeIterator->second.end()) {
		request.RequestIntegrity = UNSUPPORTED_MEDIA_TYPE;
		return;
	}
}
