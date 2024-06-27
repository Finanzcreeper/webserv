#ifndef WEBSERV_TESTS_HPP
#define WEBSERV_TESTS_HPP

#include <iostream>
#include "../interpreters/httpInterpreter.hpp"

class Tests {

private:

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
