#include "httpInterpreter.hpp"

void InterpretRequest(Request& request, const t_server& settings) {
	findRoute(request,settings);
	if (request.RequestIntegrity != OK_HTTP) {
		return;
	}
	checkIfMethodIsAllowedOnRoute(request, settings);
	if (request.RequestIntegrity != OK_HTTP) {
		return;
	}
	checkBodySize(request, settings);
	if (request.RequestIntegrity != OK_HTTP) {
		return;
	}
	redirectionChecker(request, settings);
	if (request.RequestIntegrity != OK_HTTP) {
		return;
	}
}

void findRoute(Request& request, t_server& settings) {
	std::map<std::string,route>::iterator RouteIterator;
	bool routeSet = false;

	RouteIterator = settings.routes.find(request.RequestedPath);
	if (RouteIterator != settings.routes.end()) {
		request.UsedRoute = RouteIterator->second;
		routeSet = true;
		return;
	} else {
		RouteIterator = settings.routes.begin();
		while (RouteIterator != settings.routes.end()) {
			if (request.RequestedPath.substr(0,RouteIterator->first.size()) == RouteIterator->first && request.UsedRoute.location.size() < RouteIterator->first.size()){
				request.UsedRoute = RouteIterator->second;
				routeSet = true;
			}
			++RouteIterator;
		}
	}
	if (routeSet == false) {
		request.RequestIntegrity = NOT_FOUND;
	} else {
		request.RoutedPath = request.RequestedPath.replace(0,request.UsedRoute.location.size(),request.UsedRoute.root);
	}
}

void checkIfMethodIsAllowedOnRoute(Request& request, t_server& settings) {
	if ((request.ReqType &  request.UsedRoute.methods) == 0) {
		request.RequestIntegrity = METHOD_NOT_ALLOWED;
	}
}

void checkBodySize(Request& request, const t_server& settings) {
	if (request.Body.size() > settings.client_max_body_size) {
		request.RequestIntegrity = PAYLOAD_TO_LARGE;
	}
}

void redirectionChecker(Request& request, const t_server& settings) {
	if (request.UsedRoute.httpRedirection.empty() == false) {
		request.RequestIntegrity = MOVED_PERMANENTLY;
	}
}

void checkContentType(Request& request, const t_server& settings) {
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
	if  (contentTypeFound = false) {
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
	if (contentSubtypeFound = false) {
		request.RequestIntegrity = UNSUPPORTED_MEDIA_TYPE;
	}
}