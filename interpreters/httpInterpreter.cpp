#include "httpInterpreter.hpp"

Request InterpretRequest(Request& request, const t_server& settings) {
	checkIfMethodIsAllowedOnRoute(request, settings);
	checkBodySize(request, settings);
	return (request);
}

void checkIfMethodIsAllowedOnRoute(Request& request, t_server& settings) {
	std::map<std::string,route>::iterator RouteIterator;

	RouteIterator = settings.routes.find(request.RequestedPath);
	if (RouteIterator == settings.routes.end()) {
		request.RequestIntegrity = METHOD_NOT_ALLOWED;
	} else {
		RouteIterator = settings.routes.begin();
		while (RouteIterator != settings.routes.end()) {
			if (request.RequestedPath.substr(0,RouteIterator->first.size()) == RouteIterator->first){
				if ((request.ReqType &  RouteIterator->second.methods) == 0) {
					request.RequestIntegrity = METHOD_NOT_ALLOWED;
					return;
				}
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


//void redirecttionChecker(Request& request, const t_server& settings) {
//	return;
//}