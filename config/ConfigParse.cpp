/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: siun <siun@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 22:37:44 by siun              #+#    #+#             */
/*   Updated: 2024/05/22 17:38:49 by siun             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParse.hpp"

std::string openFile(std::string path)
{
	std::ifstream	file(path);
	if (!file.is_open())
	{
		throw std::runtime_error("Could not open file " + path);
	}
	std::string	config((std::istreambuf_iterator<char>(file))
		, std::istreambuf_iterator<char>());
	file.close();
	return config;
}

std::vector<std::pair<std::string, int>>	findIndent(std::string str)
{
	std::vector<std::pair<std::string, int>> configs;
	for (int i = 0; i < str.size(); i ++)
	{
		int start = i;
		for (start; str[start] == '\t'; start ++)
			;
		int	indent = start - i;
		i = start;
		int	end = i;
		for (end; str[end] != '\n'; end ++)
			;
		i = end;
		if (start != end)
			configs.push_back(std::make_pair(str.substr(start, end - start), indent));
	}
	return configs;
}

std::vector<std::string> findChunck(std::vector<std::pair<std::string, int>> indents)
{
	//have to change auto keyword to sth else
	auto	start = std::find(indents.begin(), indents.end(), 0);
	//have to add some error handlings
	std::vector<std::string> chunck;
	for (int j = 0; j + start <  indents.end(); j ++)
	{
		auto	end = std::find(start + 1, indents.end(), 0);
		chunck.push_back(config.substr(start - indents.begin(), end - start));
		for (end; end < indents.end(); end ++)
			if (indents[end - indents.begin()] > 0)
				break ;
			//if indent level rises up from 0, it means it entered another chunck
		start = end;
	}

	// std::vector<std::string> chunck;

	// for (int i = 0; i < config.length(); i ++)
	// {
	// 	int start = config.find("server");
	// 	int end = config.find("server", start + 1);
	// 	if (end == -1)
	// 		end = config.length();
	// 	chunck.push_back(config.substr(start, end - start));
	// }
	return chunck;
}


//going to change later to handle multiple spaces
std::vector<std::string> findMethods(std::string methods)
{
	std::vector<std::string> httpMethods;
	std::string::size_type start = 0;
	std::string::size_type end = methods.find(' ');
	while (end != std::string::npos)
	{
		httpMethods.push_back(methods.substr(start, end - start));
		start = end + 1;
		end = methods.find(' ', start);
	}
	httpMethods.push_back(methods.substr(start));
	for (int i = 0; i < httpMethods.size(); i ++)
	{
		if (httpMethods[i] != "GET" || httpMethods[i] != "POST" || httpMethods[i] != "DELETE")
			throw std::runtime_error("Invalid http method: " + httpMethods[i] + " in config file");
	}
	return httpMethods;
}

std::vector<std::string> findPaths(std::string paths)
{
	std::vector<std::string> path;
	std::string::size_type start = 0;
	std::string::size_type end = paths.find(' ');
	while (end != std::string::npos)
	{
		path.push_back(paths.substr(start, end - start));
		start = end + 1;
		end = paths.find(' ', start);
	}
	path.push_back(paths.substr(start));
	return path;
}

std::map<std::string, std::string> findCgi(std::string cgi)
{
	std::map<std::string, std::string> cgi_extension;
	std::string::size_type start = 0;
	std::string::size_type end = cgi.find(' ');
	while (end != std::string::npos)
	{
		cgi_extension[cgi.substr(start, end - start)] = cgi.substr(end + 1, cgi.find(' ', end + 1) - end - 1);
		start = end + 1;
		end = cgi.find(' ', start);
	}
	cgi_extension[cgi.substr(start)] = cgi.substr(end + 1);
	return cgi_extension;
}

std::string findAndExtractValue(const std::string& chunck, const std::string& keyword) {
	size_t index = chunck.find(keyword);
	if (index != std::string::npos) {
		size_t start = index + keyword.size() + 1;
		size_t end = chunck.find('\n', start);
		return chunck.substr(start, end - start);
	}
	return "";
}

t_server parseServerConfig(const std::string& chunck) {
	t_server server;

	std::string value = findAndExtractValue(chunck, "port");
	if (!value.empty()) server.port = value;

	value = findAndExtractValue(chunck, "host");
	if (!value.empty()) server.host = value;

	value = findAndExtractValue(chunck, "server_name");
	if (!value.empty()) server.server_name = value;

	value = findAndExtractValue(chunck, "default_error_page");
	if (!value.empty()) server.default_error_page = value;

	value = findAndExtractValue(chunck, "client_max_body_size");
	if (!value.empty()) server.client_max_body_size = std::stoi(value);

	value = findAndExtractValue(chunck, "httpMethods");
	if (!value.empty()) {
		try {
			server.httpMethods = findMethods(value);
		} catch (const std::runtime_error &e){
			std::cerr << "Caught exception: " << e.what() << '\n';
		}
	}

	value = findAndExtractValue(chunck, "httpRedirection");
	if (!value.empty()) server.httpRedirection = value;

	value = findAndExtractValue(chunck, "path");
	if (!value.empty()) {
		try {
			server.path = findPaths(value);
		} catch (const std::runtime_error &e){
			std::cerr << "Caught exception: " << e.what() << '\n';
		}
	}

	value = findAndExtractValue(chunck, "cgi_extension");
	if (!value.empty()) {
		try {
			server.cgi_extension = findCgi(value);
		} catch (const std::runtime_error &e){
			std::cerr << "Caught exception: " << e.what() << '\n';
		}
	}

	value = findAndExtractValue(chunck, "dir_listing");
	if (!value.empty()) server.dir_listing = std::stoi(value);

	value = findAndExtractValue(chunck, "dir_request_default");
	if (!value.empty()) server.dir_request_default = value;

	return server;
}

//have to make loop until configPa
std::vector <t_server> configParse(std::string configFilePath)
{
	std::string				config;
	std::vector <t_server>	servers;
	std::vector<std::pair<std::string, int>> indents;
	std::vector<std::string> chuncks;
	
	try{
		config = openFile(configFilePath);
	} catch (const std::runtime_error &e){
		std::cerr << "Caught execption: " << e.what() << '\n';
		return std::vector<t_server>();
	}
	indents = findIndent(config);
	chuncks = findChunck(config, indents);
	
	// for (int i = 0; i < chuncks.size(); i ++)
	// {
	// 	try {
	// 		servers.push_back(parseServerConfig(chuncks[i]));
	// 	} catch (const std::runtime_error &e){
	// 		std::cerr << "Caught exception: " << e.what() << '\n';
	// 		return std::vector<t_server>();
	// 	}
	// }
	// return servers;
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
		for (const auto& method : server.httpMethods) {
			std::cout << method << "\n";
		}
		
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