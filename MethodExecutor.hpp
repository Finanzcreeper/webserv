#ifndef METHODEXECUTOR_HPP
#define METHODEXECUTOR_HPP

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include "Server.hpp"

class Server;
struct Response;
struct Request;

class MethodExecutor
{
	private:
		Server*	_server;
		void	_executeGet(Request &requ, Response &resp);
		//void    exexutePost();
		//void    exexuteDelete();
	public:
		MethodExecutor( void );
		MethodExecutor(Server *server);
		~MethodExecutor(){};
		void	wrapperRequest(Request &requ, Response &resp);
};

#endif