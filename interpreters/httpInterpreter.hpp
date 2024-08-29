#ifndef WEBSERV_HTTPINTERPRETER_HPP
#define WEBSERV_HTTPINTERPRETER_HPP

#include "../CommonIncludes.hpp"
#include <map>

void interpretRequest(Request& request, const t_server& settings);

void findRoute(Request& request, const t_server& settings);

void checkIfMethodIsAllowedOnRoute(Request& request);

void checkBodySize(Request& request, const t_server& settings);

void redirectionChecker(Request& request);

void checkContentType(Request& request);

void handleMultipart (Request& request);

std::string MultipartDelimiterValidation(Request& request);


#endif //WEBSERV_HTTPINTERPRETER_HPP
