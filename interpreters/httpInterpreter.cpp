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
	checkContentType(request);
	if (request.RequestIntegrity != OK_HTTP) {
		return;
	}
}

#include <iostream>
#include <algorithm>

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

	std::map<std::string,std::vector<std::string> > ContentTypeMap;
	std::map<std::string,std::vector<std::string> >::iterator ContentTypeIterator;
	std::vector<std::string> text;
	std::vector<std::string> multipart;
	std::vector<std::string>::iterator ContentSubtypeIterator;

	text.push_back("plain");
	multipart.push_back("form-data");

	ContentTypeMap.insert(std::make_pair("text/",text));
	ContentTypeMap.insert((std::make_pair("multipart/",multipart)));

	std::string foundContentType;
	std::string foundContentSubtype;

	foundContentType = headerField->second.substr(0,headerField->second.find('/') + 1);
	foundContentSubtype = headerField->second.substr(headerField->second.find('/') + 1,headerField->second.size());

	ContentTypeIterator = ContentTypeMap.find(foundContentType);
	if (ContentTypeIterator == ContentTypeMap.end()) {
		request.RequestIntegrity = UNSUPPORTED_MEDIA_TYPE;
		return;
	}
	ContentSubtypeIterator = std::find(ContentTypeIterator->second.begin(), ContentTypeIterator->second.end(), foundContentSubtype);
	if (ContentSubtypeIterator == ContentTypeIterator->second.end()) {
		request.RequestIntegrity = UNSUPPORTED_MEDIA_TYPE;
		return;
	}
}
