#ifndef WEBSERV_TESTS_HPP
#define WEBSERV_TESTS_HPP

#include <iostream>
#include <cstring>
#include "../interpreters/httpInterpreter.hpp"
#include "../parsers/httpParser.hpp"

class Tests {

private:

	connection testConnection;
	std::map<int, connection> testConnectionMap;
	std::map<int, connection>::iterator testPair;

	Request testRequest;
	t_server testSettings;
	location testLocation;

	void testHttpInterpreter();
	void testConfigParser();
	void testConfigLocationParser();
	void testHttpParser();
	void testMethodExecutor();
	void testServer();
	void testStatusCodes();
public:
	void testing();
};


#endif //WEBSERV_TESTS_HPP
