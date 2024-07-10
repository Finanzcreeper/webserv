#ifndef WEBSERV_TESTS_HPP
#define WEBSERV_TESTS_HPP

#include <iostream>
#include <cstring>
#include "../interpreters/httpInterpreter.hpp"
#include "../parsers/httpParser.hpp"

enum DebugArgs {
	SILENT = 1 << 0,		//1
	VERBOSE = 1 << 1,		//2
};

class Tests {

private:

	connection testConnection;
	std::map<int, connection> testConnectionMap;
	std::map<int, connection>::iterator testPair;

	Request testRequest;
	t_server testSettings;
	location testLocation;

	bool silent;

	void testHttpInterpreter();
	void testConfigParser();
	void testConfigLocationParser();
	void testHttpParser();
	void testMethodExecutor();
	void testServer();
	void testStatusCodes();
public:
	void testing();
	Tests(int argument);
};


#endif //WEBSERV_TESTS_HPP
