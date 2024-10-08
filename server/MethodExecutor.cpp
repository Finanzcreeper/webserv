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

MethodExecutor::MethodExecutor(const t_server *serverSettings): _serverSettings(serverSettings) , silent(false) {}

void	MethodExecutor::wrapperRequest(Request &requ, Response &resp)
{
	resp.body.clear();
	resp.responseBuffer.clear();
	resp.headerFields.clear();
	resp.httpStatus = requ.RequestIntegrity;
	if (resp.httpStatus == OK_HTTP && requ.UsedRoute.locationName == "/cgi/"){
		executeCGI(requ, resp, _serverSettings);
		if (!((int)resp.httpStatus >= MIN_CLIENT_ERROR && \
			(int)resp.httpStatus <= MAX_SERVER_ERROR)){
			resp.isReady = true;
			return ;
		}
	} else if (resp.httpStatus == MOVED_PERMANENTLY) {
		resp.headerFields["location"] = requ.UsedRoute.redirect;
	} else if (resp.httpStatus == OK_HTTP) {
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
			case (DELETE):
				_executeDelete(requ, resp);
				break ;
			default:
				break ;
		}
	}
	if ((int)resp.httpStatus >= MIN_CLIENT_ERROR && \
		(int)resp.httpStatus <= MAX_SERVER_ERROR) {
		_generateSpecialErrorFields(requ, resp);
		_generateErrorBody(resp);
	}
	_generateCommonHeaderFields(resp);
	// Append header to response
	_writeStatusLine(resp);
	_writeHeaderFields(resp);
	// Append body to response
	if (requ.ReqType != HEAD) {
		resp.responseBuffer.append(resp.body);
	}
	resp.isReady = true;
}

static void	readFile(Response& resp, std::string path, struct stat& s){
	std::ifstream	ifs(path.c_str());
		if (ifs.good()) {
			std::stringstream ss;
			ss << ifs.rdbuf();
			resp.body.append(ss.str());
			// read modification time
			struct tm* gmt = std::gmtime(&s.st_mtim.tv_sec);
			char buf[100];
    		std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmt);
			resp.headerFields["last-modified"] = std::string(buf);
		} else {
			resp.httpStatus = INTERNAL_SERVER_ERROR;
		}
		ifs.close();
}

void	MethodExecutor::_executeGet(Request &requ, Response &resp)
{
	std::string path;
    struct stat s;

    path = requ.RoutedPath;
	if (stat(path.c_str(), &s) == -1) {
		resp.httpStatus = NOT_FOUND;
		return ;
	}
	if (s.st_mode & S_IFDIR){
		if (!(s.st_mode & S_IXGRP)) {
			resp.httpStatus = UNAUTHORIZED;
		} else if (requ.UsedRoute.dirListing) {
			if (_createIndexPage(path, requ.RequestedPath, resp) == -1) {
				resp.httpStatus = INTERNAL_SERVER_ERROR;
			}
			return;
		} else if (!requ.UsedRoute.index.empty()) {
			readFile(resp, requ.UsedRoute.root + requ.UsedRoute.index, s);
		} else {
			readFile(resp, std::string("templates/index.html"), s);
		}
	} else if (s.st_mode & S_IFREG){
		if (!(s.st_mode & S_IRGRP)) {
			resp.httpStatus = UNAUTHORIZED;
			return ;
		}
		readFile(resp, path, s);
	} else {
		resp.httpStatus = NOT_FOUND;
	}
}

static void	writeFile(std::string& content, std::string path, statusCode &code){
	struct stat	s;
	// check if file already exists
	if (stat(path.c_str(), &s) == 0) {
		code = FORBIDDEN;
		return ;
	}
	std::ofstream	ofs(path.c_str(), std::ios::out);
	if (ofs.good()) {
		ofs << content;
		if(ofs.fail()) {
			code = INTERNAL_SERVER_ERROR;
		} else {
			code = CREATED;
		}
	} else {
		code = INTERNAL_SERVER_ERROR;
	}
	ofs.close();
}

void	MethodExecutor::_executePost(Request &requ, Response &resp)
{
	statusCode code;

	if (requ.HeaderFields["content-type"].find("multipart") == 0){
		std::vector<Multipart>::iterator it;
		for (it = requ.bodyParts.begin(); it != requ.bodyParts.end(); it++){
			if (it->MultipartHeaderFields.find("Content-Disposition") != it->MultipartHeaderFields.end()){
				std::string disposition = it->MultipartHeaderFields["Content-Disposition"];
				size_t	filename_pos = disposition.find("filename");
				if (filename_pos == std::string::npos){
					continue ;
				}
				std::string filename = disposition.substr(filename_pos + 10, disposition.find("\"", filename_pos + 10) - (filename_pos + 10));
				writeFile(it->Body, requ.RoutedPath + "/" + filename, code);
				resp.body.append("File \'" + requ.RequestedPath + "/" + filename + "\': " + getStatusCodeMessage(code) + "\n");
				if (resp.httpStatus == OK_HTTP && code == CREATED){
					resp.headerFields["location"] = filename;
					resp.httpStatus = CREATED;
				} else if (code == CREATED || resp.httpStatus != ACCEPTED){
					resp.httpStatus = ACCEPTED;
				} else {
					resp.httpStatus = code;
				}
			}
		}
	} else {
		writeFile(requ.Body, requ.RoutedPath, resp.httpStatus);
		if (resp.httpStatus == CREATED){
			resp.headerFields["location"] = requ.RequestedPath;
		}
		
	}
}

void	MethodExecutor::_executeDelete(Request &requ, Response &resp)
{
	std::string	path = requ.RoutedPath;
	std::string	dir_path;
	size_t pos = path.find_last_of("/");
    if (pos != std::string::npos) {
        dir_path =  path.substr(0, pos + 1);
	} else {
		dir_path = "";
	}
	struct stat	file_stat;
	struct stat dir_stat;
	if (dir_path != "" && stat(dir_path.c_str(), &dir_stat) == -1){
		resp.httpStatus = NOT_FOUND;
	} else if (!(dir_stat.st_mode & S_IRWXG)) {
		resp.httpStatus = UNAUTHORIZED;
	} else if (stat(path.c_str(), &file_stat) == -1) {
		resp.httpStatus = NOT_FOUND;
	} else if (std::remove(path.c_str()) == 0) {
		resp.httpStatus = NO_CONTENT;
	} else {
		resp.httpStatus = INTERNAL_SERVER_ERROR;
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
	for(iter = resp.headerFields.begin(); iter != resp.headerFields.end(); ++iter){
		resp.responseBuffer.append(iter->first + ": " + iter->second + "\r\n");
	}
	resp.responseBuffer.append("\r\n");
	return ;
}