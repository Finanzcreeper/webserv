#include "MethodExecutor.hpp"
#include<fstream>
#include<sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

static void	checkAndReplace(std::string& line, std::string subStr, std::string newStr);

// Creates the body when a directory is requested
int	MethodExecutor::_createIndexPage(std::string path, Response *resp)
{
	std::ifstream is((_serverSettings->workingDir \
		+ "/content/templates/dir_listing_page.html").c_str());
	if (!is.good())
	{
		resp->httpStatus = INTERNAL_SERVER_ERROR;
		return (-1);
	}
	std::string	line;
	size_t	idx_start;
	while (std::getline(is, line))
	{
		idx_start = line.find("__DIR_ENTRY__");
		if (idx_start != std::string::npos)
		{
			DIR	*dir = opendir(path.c_str());
			if (!dir)
			{
				resp->httpStatus = INTERNAL_SERVER_ERROR;
				is.close();
				return (-1);
			}
			dirent *entry = readdir(dir);
			while (entry)
			{
				resp->body.append(line.replace(idx_start, \
					std::string("__DIR_ENTRY__").length(), entry->d_name));
				entry = readdir(dir);
			}
			closedir(dir);
		}
		else
			resp->body.append(line);
	}
	is.close();
	return (0);
}

// Creates the body of the response in case an error occured
int	MethodExecutor::_generateErrorBody(Response &resp)
{
	//TODO: Check if there is an default file for an specific error
	std::stringstream statusCode_str;
	statusCode_str << resp.httpStatus;

	std::string statusCode = statusCode_str.str();
	std::string	statusMessage = getStatusCodeMessage(resp.httpStatus);
	std::string statusDescription = getStatusCodeDescription(resp.httpStatus);

	std::ifstream is((_serverSettings->workingDir \
		+ "/content/templates/error_page.html").c_str());
	if (!is.good())
	{
		resp.httpStatus = INTERNAL_SERVER_ERROR;
		return (-1);
	}
	std::string	line;
	while (std::getline(is, line))
	{
		checkAndReplace(line, "__STATUS_CODE__", statusCode);
		checkAndReplace(line, "__STATUS_MESSAGE__", statusMessage);
		checkAndReplace(line, "__STATUS_DESCRIPTION__", statusDescription);
		resp.body.append(line);
	}
	is.close();
	return (0);
}

// replaces a substring in the first argument by a new string if founded
static void	checkAndReplace(std::string& line, std::string subStr, std::string newStr)
{
	size_t	idx_start = line.find(subStr);
	if (idx_start != std::string::npos)
		line.replace(idx_start, std::string(subStr).length(), newStr);
}