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

void findRoute(Request& request,  const t_server& settings) {
	std::map<std::string, location>::const_iterator RouteIterator;
	bool routeSet = false;

	RouteIterator = settings.locations.find(request.RequestedPath);
	if (RouteIterator != settings.locations.end()) {
		request.UsedRoute = RouteIterator->second;
		routeSet = true;
	} else {
		RouteIterator = settings.locations.begin();
		while (RouteIterator != settings.locations.end()) {
			if (RouteIterator->first.size() > request.RequestedPath.size()) {
				++RouteIterator;
			}else if (request.RequestedPath.substr(0,RouteIterator->first.size()) == RouteIterator->first && request.RequestedPath.at(RouteIterator->first.size()) == '/'  && request.UsedRoute.locationName.size() < RouteIterator->first.size()){
				request.UsedRoute = RouteIterator->second;
				routeSet = true;
			}
			++RouteIterator;
		}
	}
	if (routeSet == false) {
		request.RequestIntegrity = NOT_FOUND;
	} else {
		request.RoutedPath = request.RequestedPath.replace(0,request.UsedRoute.locationName.size(),request.UsedRoute.root);
	}
}

void checkIfMethodIsAllowedOnRoute(Request& request) {
	if ((request.ReqType &  request.UsedRoute.httpMethods) == 0) {
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

	allowedContentTypes.push_back("hey/");
	allowedContentSubtypes.push_back("ho");

	std::string foundContentType;
	std::string foundContentSubtype;

	foundContentType = headerField->second.substr(0,headerField->second.find('/'));
	foundContentSubtype = headerField->second.substr(headerField->second.find('/'),headerField->second.size());
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
