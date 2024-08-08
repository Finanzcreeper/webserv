#ifndef METHODEXECUTOR_HPP
#define METHODEXECUTOR_HPP

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
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

		void	_generateErrorBody(Response &resp);
		int		_createIndexPage(std::string path, Response &resp);

	public:
		MethodExecutor( void );
		MethodExecutor(const t_server *serverSettings);
		~MethodExecutor(){};
		void	wrapperRequest(Request &requ, Response &resp);

		// unit tests
		bool silent;
		void testCheckandReplace(void);
		void testCreateIndexPage(void);
		void testGenerateErrorBody(void);

		void testGenerateCommonHeaderField(void);
		void testGenerateSpecialErrorFields(void);
		void testGenerateDateField(void);
		void testGenerateContentLengthField(void);
		void testGenerateAllowField(void);

		void testWrapperRequest(void);
		void testExecuteGet(void);
		void testExecutePost(void);
		void testExecuteDelete(void);
		void testWriteStatusLine(void);
		void testWriteHeaderFields(void);
};

#endif