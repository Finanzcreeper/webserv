#include "httpInterpreter.hpp"

void InterpretRequest(Request& request, const t_server& settings) {
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

void checkIfMethodIsAllowedOnRoute(Request& request, t_server& settings) {
	std::map<std::string,route>::iterator RouteIterator;

	RouteIterator = settings.routes.find(request.RequestedPath);
	if (RouteIterator != settings.routes.end()) {
		request.UsedRoute = RouteIterator->second;
		if ((request.ReqType &  RouteIterator->second.methods) == 0) {
			request.RequestIntegrity = METHOD_NOT_ALLOWED;
		}
		return;
	} else {
		RouteIterator = settings.routes.begin();
		while (RouteIterator != settings.routes.end()) {
			if (request.RequestedPath.substr(0,RouteIterator->first.size()) == RouteIterator->first){
				request.UsedRoute = RouteIterator->second;
				if ((request.ReqType &  RouteIterator->second.methods) == 0) {
					request.RequestIntegrity = METHOD_NOT_ALLOWED;
				}
				return;
			}
			++RouteIterator;
		}
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

void checkAndApplyRoot(Request& request, const t_server& settings) {
	std::string startOfPath;

	startOfPath =request.RequestedPath.substr(0, request.UsedRoute.root.first.size());
	if (startOfPath == request.UsedRoute.root.first) {
	request.RequestedPath.replace(0,startOfPath.size(), request.UsedRoute.root.second);
	}
}
