#ifndef WEBSERV_HTTPINTERPRETER_HPP
#define WEBSERV_HTTPINTERPRETER_HPP

#include "../CommonIncludes.hpp"
#include <map>

void InterpretRequest(Request& request, const t_server& settings);

void findRoute(Request& request, const t_server& settings);

void checkIfMethodIsAllowedOnRoute(Request& request, const t_server& settings);

void checkBodySize(Request& request, const t_server& settings);

void redirectionChecker(Request& request, const t_server& settings);

void checkContentType(Request& request, const t_server& settings);

#endif //WEBSERV_HTTPINTERPRETER_HPP
