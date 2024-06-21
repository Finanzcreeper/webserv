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
