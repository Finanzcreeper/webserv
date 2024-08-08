#include"MethodExecutor.hpp"
#include<fstream>
#include<sstream>
#include<string>
#include<cstring>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>

static void	checkAndReplace(std::string& line, std::string subStr, std::string& newStr);

// Creates the body when a directory is requested
int	MethodExecutor::_createIndexPage(std::string path, Response &resp)
{
	std::ifstream is("content/templates/dir_listing_page.html");
	if (!is.good())
	{
		resp.httpStatus = INTERNAL_SERVER_ERROR;
		return (-1);
	}
	std::string	line;
	DIR	*dir = opendir(path.c_str());
	if (!dir)
	{
		resp.httpStatus = INTERNAL_SERVER_ERROR;
		is.close();
		return (-1);
	}
	while (std::getline(is, line))
	{
		size_t	idx_start = line.find("__DIR_ENTRY__");
		if (idx_start != std::string::npos)
		{
			dirent *entry = readdir(dir);
			std::string	start = line.substr(0, idx_start);
			size_t	idx_end = idx_start + std::string("__DIR_ENTRY__").length();
			std::string	end = line.substr(idx_end, line.length() - idx_end);
			while (entry)
			{
				if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            		entry = readdir(dir);
					continue;
        		}
				resp.body.append(start);
				resp.body.append(entry->d_name);
				resp.body.append(end + "\n");
				entry = readdir(dir);
			}
			closedir(dir);
		}
		else
			resp.body.append(line + "\n");
	}
	is.close();
	return (0);
}

// Creates the body of the response in case an error occured
void	MethodExecutor::_generateErrorBody(Response &resp)
{
	std::stringstream	statusCode_str;
	statusCode_str << resp.httpStatus;
	std::map<statusCode, std::string>::const_iterator iter;
	iter = _serverSettings->errorPages.find(resp.httpStatus);
	if (iter != _serverSettings->errorPages.end())
	{
		std::ifstream	errorPage((iter->second).c_str());
		std::cout << "ERROR PAGE PATH: " << iter->second << std::endl;
		if (errorPage.good())
		{
			std::ostringstream ss;
			ss << errorPage.rdbuf();
			resp.body.append(ss.str());
			errorPage.close();
			return ;
		}
		errorPage.close();
	}
	std::string statusCode = statusCode_str.str();
	std::string	statusMessage = getStatusCodeMessage(resp.httpStatus);
	std::string statusDescription = getStatusCodeDescription(resp.httpStatus);

	std::ifstream is("content/templates/error_page.html");
	if (!is.good()) {
		return ;
	}
	std::string	line;
	while (std::getline(is, line))
	{
		checkAndReplace(line, "__STATUS_CODE__", statusCode);
		checkAndReplace(line, "__STATUS_MESSAGE__", statusMessage);
		checkAndReplace(line, "__STATUS_DESCRIPTION__", statusDescription);
		resp.body.append(line + "\n");
	}
	is.close();
}

// replaces a substring in the first argument by a new string if founded
static void	checkAndReplace(std::string& line, std::string subStr, std::string& newStr)
{
	size_t	idx_start = line.find(subStr);
	if (idx_start != std::string::npos)
		line.replace(idx_start, std::string(subStr).length(), newStr);
}

void MethodExecutor::testCheckandReplace(void) {
	std::string	line;
	std::string	subStr;
	std::string newString;
	std::string expectedOutput;
	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	line = "If the word: \"HALLO\" is equal to the word SUCCESS, it worked out";
	subStr = "HALLO";
	newString = "SUCCESS";
	expectedOutput = "If the word: \"SUCCESS\" is equal to the word SUCCESS, it worked out";
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	checkAndReplace(line, subStr, newString);
	if (expectedOutput != line) {
		std::cout << "Replace simple substring: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Replace simple substring: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	line = "This line should not be changed";
	subStr = "HALLO";
	newString = "SUCCESS";
	expectedOutput = "This line should not be changed";
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	checkAndReplace(line, subStr, newString);
	if (expectedOutput != line) {
		std::cout << "No replace possible: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "No replace possible: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 3-------------------//
	//==========================================================//
	line = "";
	subStr = "bla";
	newString = "test";
	expectedOutput = "";
	//----------------------------------------------------------//
	//======================Running Test 3======================//
	//----------------------------------------------------------//
	checkAndReplace(line, subStr, newString);
	if (expectedOutput != line) {
		std::cout << "Empty string inputs: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Empty string inputs: \033[1;32mOK\033[0m" << std::endl;
	}
}