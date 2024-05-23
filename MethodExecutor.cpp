#include "MethodExecutor.hpp"

MethodExecutor::MethodExecutor( void ): _server(0) {};

MethodExecutor::MethodExecutor(Server *server)
{
	_server = server;
}

void	MethodExecutor::wrapperRequest(Request &requ, Response &resp)
{
	switch (requ.ReqType){
		case (GET):
			_executeGet(requ, resp);
		//case (POST):
		//	_executePost(requ, resp);
		//case (DELETE):
		//	_executeDelete(requ, resp);
		default:
			std::cerr << "Method type not found\n";
	}
}

void    MethodExecutor::_executeGet(Request &requ, Response &resp)
{
	std::ifstream	is;
	std::string		path = "/home/thofting/repos/webserv_creeper/content";
	//std::string		filename = "example.html";
	
	is.open((path + "/" + requ.filename).c_str());
	if (is.is_open())
		is >> resp.ResponseBuffer;
}