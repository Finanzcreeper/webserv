#include "MethodExecutor.hpp"
#include<fstream>
#include<sstream>
#include <ctime>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <iostream>

std::string		defaultPage = "index.html";

MethodExecutor::MethodExecutor( void ): _serverSettings(0) {
};

MethodExecutor::MethodExecutor(const t_server *serverSettings): _serverSettings(serverSettings) {}

void	MethodExecutor::wrapperRequest(Request &requ, Response &resp)
{
	std::cout << "Used route: " + requ.UsedRoute.locationName << std::endl;
	std::cout << "Used redirect: " + requ.UsedRoute.root << std::endl;
	std::cout << "Used path: " + requ.RoutedPath << std::endl;
	//std::cout << "**** HEADER OF REQUEST: ****\n" << requ.HeaderBuffer << std::endl << "**** END OF HEADER ****" << std::endl;
	//std::cout << "**** BODY OF REQUEST: ****\n" << requ.Body << std::endl << "**** END OF BODY ****" << std::endl;
	std::cout << "REQUEST TYPE: " << requ.ReqType << std::endl;
	resp.body.clear();
	resp.responseBuffer.clear();
	resp.headerFields.clear();
	resp.httpStatus = requ.RequestIntegrity;
	requ.HeaderFields["content-type"] = "text/plain";

	if (resp.httpStatus == MOVED_PERMANENTLY)
		resp.headerFields["location"] = requ.UsedRoute.redirect;
	else if (resp.httpStatus == OK_HTTP)
	{
		switch (requ.ReqType){
			case (GET):
				_executeGet(requ, resp);
				break ;
			case (HEAD):
				_executeGet(requ, resp);
				break ;
			case (POST):
				_executePost(requ, resp);
				break ;
			//case (DELETE):
			//	_executeDelete(requ, resp);
			//	break ;
			default:
				std::cerr << "Method type not found\n";
		}
	}
	if ((int)resp.httpStatus >= MIN_CLIENT_ERROR && (int)resp.httpStatus <= MAX_SERVER_ERROR)
	{
		_generateSpecialErrorFields(requ, resp);
		_generateErrorBody(resp);
	}
	_generateCommonHeaderFields(resp);
	// Append header to response
	_writeStatusLine(resp);
	_writeHeaderFields(resp);
	// Append body to response
	if (requ.ReqType != HEAD)
		resp.responseBuffer.append(resp.body);
	resp.isReady = true;
	std::cout << "**** RESPONSE: ****\n" << resp.responseBuffer << "**** END OF RESPONSE ****" << std::endl;
}

void	MethodExecutor::_executePost(Request &requ, Response &resp)
{
	std::string	path = requ.RoutedPath;
	struct stat	s;
	// check if file already exists
	if (stat(path.c_str(), &s) == 0)
	{
		resp.httpStatus = FORBIDDEN;
		return ;
	}
	std::ofstream	ofs(path.c_str(), std::ios::out);
	if (ofs.good())
	{
		ofs << requ.Body;
		if(ofs.fail())
		{
			std::cout << "Error while creating requested file: \'" + requ.RoutedPath + "\'"<< std::endl;
			resp.httpStatus = INTERNAL_SERVER_ERROR;
		}
		else
		{
			resp.httpStatus = CREATED;
			resp.headerFields["location"] = requ.RequestedPath;
		}
	}
	else
	{
		std::cout << "Error while creating requested file: \'" + requ.RoutedPath + "\'"<< std::endl;
		resp.httpStatus = INTERNAL_SERVER_ERROR;
	}
	ofs.close();
}

void	MethodExecutor::_executeGet(Request &requ, Response &resp)
{
	std::string	path = requ.RoutedPath;
	struct stat	s;

	// default page if no path specified
	if (path.length() == _serverSettings->workingDir.length() + 1)
		path.append(requ.UsedRoute.index);
	if (stat(path.c_str(), &s) == -1)
	{
		resp.httpStatus = NOT_FOUND;
		return ;
	}
	if (!(s.st_mode & S_IRGRP))
		resp.httpStatus = UNAUTHORIZED;
	else if ((s.st_mode & S_IFDIR))
	{
		if (requ.UsedRoute.dirListing)
		{
			if (_createIndexPage(path, resp) == -1)
				resp.httpStatus = INTERNAL_SERVER_ERROR;
			return;
		}
		std::cout << path + " is directory!" << std::endl;
		resp.httpStatus = NOT_FOUND;
	}
	else if (s.st_mode & S_IFREG)
	{
		std::ifstream	ifs(path.c_str());
		if (ifs.good())
		{
			std::ostringstream ss;
			ss << ifs.rdbuf();
			resp.body.append(ss.str());

			// read modification time
			struct tm* gmt = std::gmtime(&s.st_mtim.tv_sec);
			char buf[100];
    		std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmt);
			resp.headerFields["last-modified"] = std::string(buf);
		}
		else
		{
			std::cout << "Error while opening requested file: \'" + requ.RoutedPath + "\'"<< std::endl;
			resp.httpStatus = INTERNAL_SERVER_ERROR;
		}
		ifs.close();
	}
}

// creates the status line for an http response and adds it to the output buffer
void	MethodExecutor::_writeStatusLine(Response &resp)
{
	std::stringstream statusCode_str;
	statusCode_str << resp.httpStatus;

	resp.responseBuffer.append(std::string(HTTP_PROTOCOL) + " ");
	resp.responseBuffer.append(statusCode_str.str()+ " ");
	resp.responseBuffer.append(getStatusCodeMessage(resp.httpStatus) + "\n");
}

// adds all header fields which were filled to the output buffer
void	MethodExecutor::_writeHeaderFields(Response &resp)
{
	std::map<std::string,std::string>::iterator iter;
	for(iter = resp.headerFields.begin(); iter != resp.headerFields.end(); ++iter)
		resp.responseBuffer.append(iter->first + ": " + iter->second + "\n");
	resp.responseBuffer.append("\r\n\r\n");
	return ;
}