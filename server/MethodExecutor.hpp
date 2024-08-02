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
		void			_writeStatusLine(Response &resp);
		void			_writeHeaderFields(Response &resp);
		void			_generateCommonHeaderFields(Response &resp);
		void			_generateSpecialErrorFields(Request &req, Response &resp);

		int		_generateErrorBody(Response &resp);
		int		_createIndexPage(std::string path, Response &resp);

	public:
		MethodExecutor( void );
		MethodExecutor(const t_server *serverSettings);
		~MethodExecutor(){};
		void	wrapperRequest(Request &requ, Response &resp);
};

#endif