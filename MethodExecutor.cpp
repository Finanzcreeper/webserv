#include "MethodExecutor.hpp"
#include<fstream>
#include<sstream>

std::string		root = "/home/thofting/repos/webserv_creeper/content";
std::string		defaultPage = "index.html";

MethodExecutor::MethodExecutor( void ): _server(0) {};

MethodExecutor::MethodExecutor(Server *server)
{
	_server = server;
}

void	MethodExecutor::wrapperRequest(Request &requ, Response &resp)
{
	std::cout << "**** HEADER OF REQUEST: ****\n" << requ.HeaderBuffer << std::endl << "**** END OF HEADER ****" << std::endl;
	resp.statusCode = OK_HTTP;
	resp.body.clear();
	resp.headerBuffer.clear();
	resp.responseBuffer.clear();
	switch (requ.ReqType){
		case (GET):
			_executeGet(requ, resp);
			break ;
		//case (HEAD):
		//	_executeHead(requ, resp);
		//	break ;
		//case (POST):
		//	_executePost(requ, resp);
		//	break ;
		//case (DELETE):
		//	_executeDelete(requ, resp);
		//	break ;
		default:
			std::cerr << "Method type not found\n";
			resp.statusCode = METHOD_NOT_ALLOWED;
	}
	if ((int)resp.statusCode >= MIN_CLIENT_ERROR && (int)resp.statusCode <= MAX_SERVER_ERROR)
		_generateErrorBody(resp);
	_generateHeader(requ, resp);

	//Body header separation
	resp.responseBuffer.append("\r\n\r\n");
	resp.responseBuffer.append(resp.body);
	std::cout << "**** RESPONSE: ****\n" << resp.responseBuffer << "**** END OF RESPONSE ****" << std::endl;
}

void    MethodExecutor::_executeGet(Request &requ, Response &resp)
{
	std::string		path = root + requ.RequestedPath;

	// removing trailing whitespace, could be done during parsing as well
	size_t			pos = path.find(" ");
	path.replace(pos, 1, "");

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
		resp.statusCode = NOT_FOUND;
	}
	ifs.close();
}

void	MethodExecutor::_generateHeader(Request &requ, Response &resp)
{
	std::string	vProtocol = "HTTP/1.1";
	int	temp = requ.Body.length();
	temp++;

	// First line:
	resp.responseBuffer.append(vProtocol
		+ " " + std::to_string(resp.statusCode)
		+ " " + getStatusCodeMessage(resp.statusCode) + "\n");
	resp.responseBuffer.append("Content-Length: "
		+ std::to_string(resp.body.length()));
	return ;
}

void	MethodExecutor::_generateErrorBody(Response &resp)
{
	std::ifstream	ifs;
	std::string		errorBody;
	
	resp.body.append("<!DOCTYPE html>\n<html lang=\"en\">\n");
	resp.body.append("<head>\n");
	resp.body.append("	<meta charset=\"UTF-8\">\n");
    resp.body.append("	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    resp.body.append("	<title>");
	resp.body.append(std::to_string(resp.statusCode));
	resp.body.append(" ");
	resp.body.append(getStatusCodeMessage(resp.statusCode));
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
	resp.body.append(std::to_string(resp.statusCode));
	resp.body.append(" ");
	resp.body.append(getStatusCodeMessage(resp.statusCode));
	resp.body.append( + " </h1>\n");
	resp.body.append("        <p> ");
	resp.body.append(getStatusCodeDescription(resp.statusCode));
	resp.body.append(" </p>\n");
	resp.body.append("        <p><a href=\"/home/thofting/repos/webserv_creeper\">Go to Homepage</a></p>\n");
	resp.body.append("    </div>");
	resp.body.append("</body>");
	resp.body.append("</html>\n");
}