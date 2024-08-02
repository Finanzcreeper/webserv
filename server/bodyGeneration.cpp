#include "MethodExecutor.hpp"
#include<fstream>
#include<sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

static void	checkAndReplace(std::string& line, std::string subStr, std::string newStr);

// Creates the body when a directory is requested
int	MethodExecutor::_createIndexPage(std::string path, Response &resp)
{
	std::ifstream is((_serverSettings->workingDir \
		+ "/content/templates/dir_listing_page.html").c_str());
	if (!is.good())
	{
		resp.httpStatus = INTERNAL_SERVER_ERROR;
		return (-1);
	}
	std::string	line;
	while (std::getline(is, line))
	{
		size_t	idx_start = line.find("__DIR_ENTRY__");
		if (idx_start != std::string::npos)
		{
			DIR	*dir = opendir(path.c_str());
			if (!dir)
			{
				resp.httpStatus = INTERNAL_SERVER_ERROR;
				is.close();
				return (-1);
			}
			dirent *entry = readdir(dir);
			std::string	start = line.substr(0, idx_start);
			size_t	idx_end = idx_start + std::string("__DIR_ENTRY__").length();
			std::string	end = line.substr(idx_end, line.length() - idx_end);
			while (entry)
			{
				resp.body.append(start);
				resp.body.append(entry->d_name);
				resp.body.append(end);
				entry = readdir(dir);
			}
			closedir(dir);
		}
		else
			resp.body.append(line);
	}
	is.close();
	return (0);
}

// Creates the body of the response in case an error occured
int	MethodExecutor::_generateErrorBody(Response &resp)
{
	std::stringstream	statusCode_str;
	statusCode_str << resp.httpStatus;
	std::map<statusCode, std::string>::const_iterator iter;
	iter = _serverSettings->errorPages.find(resp.httpStatus);
	if (iter != _serverSettings->errorPages.end())
	{
		std::ifstream	errorPage((_serverSettings->workingDir \
			+ iter->second).c_str());
		std::cout << "ERROR PAGE PATH: " << _serverSettings->workingDir \
			+ iter->second << std::endl;
		if (errorPage.good())
		{
			std::ostringstream ss;
			ss << errorPage.rdbuf();
			resp.body.append(ss.str());
			errorPage.close();
			return (0);
		}
		errorPage.close();
	}
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