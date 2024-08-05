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
	resp.headerFields["server"] = _serverSettings->serverName;
	resp.headerFields["date"] = generateDateField();
	resp.headerFields["accept-ranges"] = "0";
	resp.headerFields["content-length"] = generateContentLengthField(resp.body);
}

// adds fields to the header of responses in case an error occured
void	MethodExecutor::_generateSpecialErrorFields(Request &requ, Response &resp)
{
	if (resp.httpStatus == METHOD_NOT_ALLOWED)
		resp.headerFields["allow"] = generateAllowField(requ.UsedRoute.httpMethods);
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

void MethodExecutor::testGenerateDateField(void) {
	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	std::string	date;
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	date = generateDateField();
	if (date.length() != 29) {
		std::cout << "Time format correct: \033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "Time format correct: \033[1;32mOK\033[0m" << std::endl;
	}
}

void MethodExecutor::testGenerateContentLengthField(void){
	std::string	body;
	std::string length;
	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	body = "this string has 29 characters";
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	length = generateContentLengthField(body);
	if (length != "29") {
		std::cout << "Simple length: \033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "Simple length: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	body = "";
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	length = generateContentLengthField(body);
	if (length != "0") {
		std::cout << "Empty string: \033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "Empty string: \033[1;32mOK\033[0m" << std::endl;
	}
}

void MethodExecutor::testGenerateAllowField(void) {
	int			allowedMethods;
	std::string	output;
	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	allowedMethods = 0;
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	output = generateAllowField(allowedMethods);
	if (output != "") {
		std::cout << "No methods allowed: \033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "No methods allowed: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	allowedMethods = GET | POST;
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	output = generateAllowField(allowedMethods);
	if (output != "GET POST") {
		std::cout << "Get and Post: \033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "Get and Post: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 3-------------------//
	//==========================================================//
	allowedMethods = GET | HEAD| POST | DELETE | PUT | CONNECT | OPTIONS | TRACE | PATCH;
	//----------------------------------------------------------//
	//======================Running Test 3======================//
	//----------------------------------------------------------//
	output = generateAllowField(allowedMethods);
	if (output != "GET HEAD POST PUT DELETE CONNECT OPTIONS TRACE PATCH") {
		std::cout << "All methods: \033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "All methods: \033[1;32mOK\033[0m" << std::endl;
	}
}