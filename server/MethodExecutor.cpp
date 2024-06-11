#include "MethodExecutor.hpp"
#include<fstream>
#include<sstream>
#include <ctime>
#include <string>

std::string		root = "/home/thofting/repos/webserv_creeper/content";
std::string		defaultPage = "index.html";
std::string		vProtocol = "HTTP/1.1";

MethodExecutor::MethodExecutor( void ): _serverSettings(0) {};

MethodExecutor::MethodExecutor(const t_server *serverSettings): _serverSettings(serverSettings) {}

void	MethodExecutor::_generateCommonHeaderFields(Response &resp)
{
	resp.headerFields["Server"] = "webserv";

	// date
	std::time_t now = std::time(0);
    std::tm* gmt = std::gmtime(&now);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmt);
	resp.headerFields["Date"] = std::string(buffer);

	resp.headerFields["Accept-Ranges"] = "none"; // Indicated that range requests are not allowed
	resp.headerFields["Accept-Ranges"] = "0";

	// entity fields
	std::stringstream bodyLength;
	bodyLength << resp.body.length();
	resp.headerFields["Content-Length"] = bodyLength.str();
}

void	MethodExecutor::_generateSpecialErrorFields(Request &requ, Response &resp)
{
	if (requ.RequestIntegrity == METHOD_NOT_ALLOWED)
		resp.headerFields["Allow"] = "Hardcoded GET PUT DELETE";
}

void	MethodExecutor::wrapperRequest(Request &requ, Response &resp)
{
	std::cout << "**** HEADER OF REQUEST: ****\n" << requ.HeaderBuffer << std::endl << "**** END OF HEADER ****" << std::endl;
	resp.body.clear();
	resp.responseBuffer.clear();
	resp.headerFields.clear();
	resp.httpStatus = requ.RequestIntegrity;
	if (resp.httpStatus == MOVED_PERMANENTLY)
		resp.headerFields["Location"] = _serverSettings->httpMethods;//"tbd: Redirection location as defined in routing struct";
	else if (resp.httpStatus == OK_HTTP)
	{
		switch (requ.ReqType){
			case (GET):
				_executeGet(requ, resp);
				break ;
			case (HEAD):
				_executeGet(requ, resp); // Same procedure as for GET, but produced body is not sended to client
				break ;
			//case (POST):
			//	_executePost(requ, resp);
			//	break ;
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

void    MethodExecutor::_executeGet(Request &requ, Response &resp)
{
	std::string		path = root + requ.RequestedPath;

	// default page if no path specified
	if (path.length() == root.length() + 1)
		path.append(defaultPage);
	
	std::cout << "PATH:" << path << "*" << std::endl;
	std::ifstream	ifs(path.c_str());
	
	// TODO: More detailed access check for files
	// - Check if file exists
	if (ifs.good())
	{
		std::ostringstream ss;
      	ss << ifs.rdbuf();
		resp.body.append(ss.str());
	}
	else
	{
		std::cout << "Error while opening requested file: \'" + root
			+ requ.RequestedPath + "\'"<< std::endl;
		resp.httpStatus = NOT_FOUND;
	}
	ifs.close();
}

void	MethodExecutor::_writeStatusLine(Response &resp)
{
	std::stringstream statusCode_str;
	statusCode_str << resp.httpStatus;

	resp.responseBuffer.append(vProtocol
							   + " " + statusCode_str.str()
							   + " " + getStatusCodeMessage(resp.httpStatus) + "\n");
}

void	MethodExecutor::_writeHeaderFields(Response &resp)
{
	std::map<std::string,std::string>::iterator iter;
	for(iter = resp.headerFields.begin(); iter != resp.headerFields.end(); ++iter)
		resp.responseBuffer.append(iter->first + ": " + iter->second + "\n");
	resp.responseBuffer.append("\r\n\r\n");
	return ;
}

void	MethodExecutor::_generateErrorBody(Response &resp)
{
	std::ifstream	ifs;
	std::string		errorBody;
	std::stringstream statusCode_str;
	statusCode_str << resp.httpStatus;

	resp.body.clear();
	resp.body.append("<!DOCTYPE html>\n<html lang=\"en\">\n");
	resp.body.append("<head>\n");
	resp.body.append("	<meta charset=\"UTF-8\">\n");
    resp.body.append("	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    resp.body.append("	<title>");
	resp.body.append(statusCode_str.str());
	resp.body.append(" ");
	resp.body.append(getStatusCodeMessage(resp.httpStatus));
	resp.body.append(" </title>\n");
    resp.body.append("	<style>\n");
    resp.body.append("	    body {\n");
    resp.body.append("	        font-family: Arial, sans-serif;\n");
    resp.body.append("	        background-color: #f8f8f8;\n");
    resp.body.append("	        color: #333;\n");
    resp.body.append("	        display: flex;\n");
    resp.body.append("	        justify-content: center;\n");
    resp.body.append("	        align-items: center;\n");
    resp.body.append("	        height: 100vh;\n");
    resp.body.append("	        margin: 0;\n");
    resp.body.append("	    }\n");
    resp.body.append("	    .container {\n");
    resp.body.append("	        text-align: center;\n");
    resp.body.append("	    }\n");
    resp.body.append("	    h1 {\n");
    resp.body.append("	        font-size: 3em;\n");
    resp.body.append("	        margin-bottom: 0.5em;\n");
    resp.body.append("	    }\n");
    resp.body.append("	    p {\n");
    resp.body.append("	        font-size: 1.2em;\n");
    resp.body.append("	    }\n");
    resp.body.append("	    a {\n");
    resp.body.append("	        color: #007bff;\n");
    resp.body.append("	        text-decoration: none;\n");
    resp.body.append("	    }\n");
    resp.body.append("	    a:hover {\n");
    resp.body.append("	        text-decoration: underline;\n");
    resp.body.append("	    }\n");
    resp.body.append("	</style>\n");
	resp.body.append("</head>\n");
	resp.body.append("<body>\n");
	resp.body.append("    <div class=\"container\">\n");
	resp.body.append("        <h1>");
	resp.body.append(statusCode_str.str());
	resp.body.append(" ");
	resp.body.append(getStatusCodeMessage(resp.httpStatus));
	resp.body.append( + " </h1>\n");
	resp.body.append("        <p> ");
	resp.body.append(getStatusCodeDescription(resp.httpStatus));
	resp.body.append(" </p>\n");
	resp.body.append("        <p><a href=\"/home/thofting/repos/webserv_creeper\">Go to Homepage</a></p>\n");
	resp.body.append("    </div>");
	resp.body.append("</body>");
	resp.body.append("</html>\n");
}