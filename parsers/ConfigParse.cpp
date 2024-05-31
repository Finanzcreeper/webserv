/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: subpark <subpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 22:37:44 by siun              #+#    #+#             */
/*   Updated: 2024/05/31 18:27:01 by subpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParse.hpp"
#include "../CommonIncludes.hpp"

std::string openFile(std::string path)
{
	std::ifstream	file(path.c_str());

	if (!file.is_open())
	{
		throw std::runtime_error("Could not open file " + path);
	}
	std::string	config((std::istreambuf_iterator<char>(file))
		, std::istreambuf_iterator<char>());
	file.close();
	return config;
}

std::vector<std::pair<std::string, int> >	findIndent(std::string str)
{
	std::vector<std::pair<std::string, int> > configs;
	for (size_t i = 0; i < str.size(); i ++) {
		int start = i;
		while (str[start] == '\t') {
			++start;
		}
		int indent = start - i;
		i = start;
		int end = i;
		while (str[end] != '\n') {
			++end;
		}
		i = end;
		if (start != end)
			configs.push_back(std::make_pair(str.substr(start, end - start), indent));
	}
	return configs;
}

std::vector<std::vector<std::pair<std::string, int> > > findChunck(std::vector<std::pair<std::string, int> > indents)
{
	std::vector<std::vector<std::pair<std::string, int> > > multiChunck;
	size_t start = 0;
	size_t	end = 0;
	
	for (size_t i = 0; i < indents.size(); i ++)
	{
		start = i;
		while (i < indents.size() && indents[i].first != "server") {
			++i;
		}
		end = i;
		if (start != end)
		{
			std::vector<std::pair<std::string, int> > chunck(indents.begin() + start, indents.begin() + end);
			multiChunck.push_back(chunck);
		}
	}
	return multiChunck;
}

std::string	parseString(const std::vector<std::pair<std::string, int> > chunck, std::string keyword)
{
	for (size_t i = 0; i < chunck.size(); i ++)
	{
		if (chunck[i].first.find(keyword) != std::string::npos)
		{
			std::string str = chunck[i].first;
			std::string::iterator start = str.begin() + keyword.size() + 1;
			while(start != str.end() && std::isspace(*start))
				++ start;
			std::string::iterator end = str.end();
			while (end != start && std::isspace(*end))
				-- end;
			return (std::string(start, end + 1));
		}
	}
	return "";
}

int parseMethod(const std::vector<std::pair<std::string, int> >& chunck) {
	int methods;
	std::string str;
	std::string method;

	str = parseString(chunck,"httpMethods");
	std::istringstream stream(str);
	while(stream >> method) {
		if (method.find("GET") == 0) {
			methods = methods | GET;
		} else if (method == "HEAD") {
			methods = methods | HEAD;
		} else if (method == "POST") {
			methods = methods | POST;
		} else if (method == "PUT") {
			methods = methods | PUT;
		} else if (method == "DELETE") {
			methods = methods | DELETE;
		} else if (method == "CONNECT") {
			methods = methods | CONNECT;
		} else if (method == "OPTIONS") {
			methods = methods | OPTIONS;
		} else if (method == "TRACE") {
			methods = methods | TRACE;
		} else if (method == "PATCH") {
			methods = methods | PATCH;
		} else {
			methods = methods | INVALID;
		}
	}
	return (methods);
}

std::vector<std::string>	parsePath(const std::vector<std::pair<std::string, int> > chunck)
{
	std::vector<std::string> paths;
	std::string				keyword = "path";
	size_t	i = 0;

	while (i < chunck.size() && chunck[i].first.find(keyword) == std::string::npos) {
		++i;
	}
	int	indent = chunck[i].second;
	i++;
	while (i < chunck.size() && chunck[i].second == indent + 1) {
		std::istringstream iss(chunck[i].first);
		std::string path;
		iss >> path;
		paths.push_back(path);
		++i;
	}
	return paths;
}

std::map<std::string, std::string> parseCgi(std::vector<std::pair<std::string, int> > chunck)
{
	std::map<std::string, std::string> cgi;
	std::string keyword = "cgi";
	size_t	i = 0;

	while (i < chunck.size() && chunck[i].first.find(keyword) == std::string::npos) {
		++i;
	}
	int	indent = chunck[i].second;
	i ++;
	while (i < chunck.size() && chunck[i].second == indent + 1) {
		std::istringstream iss(chunck[i].first);
		std::string extension;
		std::string path;
		iss >> extension >> path;
		cgi[extension] = path;
		++i;
	}
	return cgi;
}

t_server parseServerConfig(const std::vector<std::pair<std::string, int> > chunck) {

	t_server server;

	server.port = parseString(chunck, "port");
	server.host = parseString(chunck, "host");
	server.server_name = parseString(chunck, "server_name");
	server.default_error_page = parseString(chunck, "default_error_page");
	server.client_max_body_size = std::atoi(parseString(chunck, "client_max_body_size").c_str());
	server.httpMethods = parseMethod(chunck);
	server.httpRedirection = parseString(chunck, "httpRedirection");
	server.path = parsePath(chunck);
	server.cgi_extension = parseCgi(chunck);
	server.dir_listing = std::atoi(parseString(chunck, "dir_listing").c_str());
	server.dir_request_default = parseString(chunck, "dir_request_default");
	return server;
}

//have to make loop until configPa
std::vector <t_server> configParse(std::string configFilePath)
{
	std::string				config;
	std::vector <t_server>	servers;
	std::vector<std::pair<std::string, int> > indents;
	std::vector<std::vector<std::pair<std::string, int> > > chuncks;

	try{
		config = openFile(configFilePath);
	} catch (const std::runtime_error &e){
		std::cerr << "Caught execption: " << e.what() << '\n';
		return std::vector<t_server>();
	}
	indents = findIndent(config);
	chuncks = findChunck(indents);
	for (size_t i = 0; i < chuncks.size(); i ++)
	{
		try {
			servers.push_back(parseServerConfig(chuncks[i]));
		} catch (const std::runtime_error &e){
			std::cerr << "Caught exception: " << e.what() << '\n';
			return std::vector<t_server>();
		}
	}
	return servers;
	return std::vector<t_server>();
}

int main()
{
	std::vector<t_server> servers = configParse("sampleConfig.conf");


	for (const auto& server : servers) {
		std::cout << "Server Configuration:\n";
		std::cout << "Port: " << server.port << "\n";
		std::cout << "Host: " << server.host << "\n";
		std::cout << "Server Name: " << server.server_name << "\n";
		std::cout << "Default Error Page: " << server.default_error_page << "\n";
		std::cout << "Client Max Body Size: " << server.client_max_body_size << "\n";
		
		std::cout << "HTTP Methods:\n";
		// for (const auto& method : server.httpMethods) {
		// 	std::cout << method << "\t";
		// }
		std::cout << "\n";
		
		std::cout << "HTTP Redirection: " << server.httpRedirection << "\n";
		
		std::cout << "Paths:\n";
		for (const auto& path : server.path) {
			std::cout << path << "\n";
		}
		
		std::cout << "CGI Extensions:\n";
		for (const auto& cgi : server.cgi_extension) {
			std::cout << "Extension: " << cgi.first << ", Path: " << cgi.second << "\n";
		}
		
		std::cout << "Directory Listing: " << server.dir_listing << "\n";
		std::cout << "Default Directory Request: " << server.dir_request_default << "\n";
		
		std::cout << "------------------------\n";
	}
}