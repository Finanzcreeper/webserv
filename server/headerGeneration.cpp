#include "MethodExecutor.hpp"
#include<fstream>
#include<sstream>
#include <ctime>
#include <string>

static std::string	generateDateField(void);
static std::string	generateContentLengthField(std::string& body);
static std::string	generateAllowField(int allowedTypes);

// adds fields to the header which are shared by all responses
void	MethodExecutor::_generateCommonHeaderFields(Response &resp)
{
	resp.headerFields["Server"] = _serverSettings->serverName;
	resp.headerFields["Date"] = generateDateField();
	resp.headerFields["Accept-Ranges"] = "0";
	resp.headerFields["Content-Length"] = generateContentLengthField(resp.body);
}

// adds fields to the header of responses in case an error occured
void	MethodExecutor::_generateSpecialErrorFields(Request &requ, Response &resp)
{
	if (resp.httpStatus == METHOD_NOT_ALLOWED)
		resp.headerFields["Allow"] = generateAllowField(requ.UsedRoute.httpMethods);
}

static std::string	generateDateField(void)
{
	std::time_t now = std::time(0);
    std::tm* gmt = std::gmtime(&now);
    char buf[100];
    std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmt);
	return (std::string(buf));
}

static std::string	generateContentLengthField(std::string& body)
{
	std::stringstream bodyLength;
	bodyLength << body.length();
	return (bodyLength.str());
}

static std::string	generateAllowField(int allowedTypes)
{
	std::string typesAsString;
	if (allowedTypes & NONE)
		typesAsString.append(" NONE");
	if (allowedTypes & INVALID)
		typesAsString.append(" INVALID");
	if (allowedTypes & GET)
		typesAsString.append(" GET");
	if (allowedTypes & HEAD)
		typesAsString.append(" HEAD");
	if (allowedTypes & POST)
		typesAsString.append(" POST");
	if (allowedTypes & PUT)
		typesAsString.append(" PUT");
	if (allowedTypes & DELETE)
		typesAsString.append(" DELETE");
	if (allowedTypes & CONNECT)
		typesAsString.append(" CONNECT");
	if (allowedTypes & OPTIONS)
		typesAsString.append(" OPTIONS");
	if (allowedTypes & TRACE)
		typesAsString.append(" TRACE");
	if (allowedTypes & PATCH)
		typesAsString.append(" PATCH");
	if (*typesAsString.begin() == ' ')
		typesAsString.erase(typesAsString.begin());
	return (typesAsString);
}