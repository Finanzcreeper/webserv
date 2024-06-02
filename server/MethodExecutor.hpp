#ifndef METHODEXECUTOR_HPP
#define METHODEXECUTOR_HPP

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include "Server.hpp"
#include "../CommonIncludes.hpp"

class MethodExecutor
{
	private:
		const t_server*	_serverSettings;
		void			_executeGet(Request &requ, Response &resp);
		void			_executePost(Request &requ, Response &resp);
		void			_executeDelete(Request &requ, Response &resp);
		void			_writeHeader(Response &resp);
		void			_generateCommonHeaderFields(Response &resp);
		void			_generateErrorBody(Response &resp);

		// Header field generation
		void			_setAllowField(Response &resp);
	public:
		MethodExecutor( void );
		MethodExecutor(const t_server *serverSettings);
		~MethodExecutor(){};
		void	wrapperRequest(Request &requ, Response &resp);
};

#endif