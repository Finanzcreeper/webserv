#include "MethodExecutor.hpp"

MethodExecutor::MethodExecutor( void ): _server(0) {};

MethodExecutor::MethodExecutor(Server *server)
{
	_server = server;
}

void	MethodExecutor::wrapperRequest(Request &requ, Response &resp)
{
	std::cout << "HEADER OF METHOD:\n\n" << requ.HeaderBuffer << std::endl << std::endl;
	std::cout << "PATH: \"" << requ.RequestedPath << "\"" << std::endl;
	switch (requ.ReqType){
		case (GET):
			_executeGet(requ, resp);
			break ;
		//case (POST):
		//	_executePost(requ, resp);
			break ;
		//case (DELETE):
		//	_executeDelete(requ, resp);
			break ;
		default:
			std::cerr << "Method type not found\n";
	}
}

void    MethodExecutor::_executeGet(Request &requ, Response &resp)
{
	std::ifstream	is;
	std::string		path = "/home/thofting/repos/webserv_creeper/content";
	

	is.open((path + "/" + requ.RequestedPath).c_str());
	if (is.is_open())
		is >> resp.ResponseBuffer;
}