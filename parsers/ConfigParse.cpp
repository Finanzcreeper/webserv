/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: subpark <subpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 22:37:44 by siun              #+#    #+#             */
/*   Updated: 2024/06/21 20:59:16 by subpark          ###   ########.fr       */
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

std::string nth_word(std::string str, int n)
{
	if (str.empty())
		return "";

	std::istringstream stream(str);
	std::string word;
	
	for (int i = 0; i < n; i ++)
	{
		stream >> word;
	}
	return word;
}

std::vector<std::pair<std::string, int> >	findIndent(std::string str)
{
	std::vector<std::pair<std::string, int> > configs;

	for (size_t i = 0; i < str.size(); i ++) {
		int start = i;
		while (str[start] && (str[start] == '\t' || str[start] == ' ')) {
			++start;
		}
		int indent = start - i;
		i = start;
		int end = i;
		while (str[end] && str[end] != '\n') {
			++end;
		}
		i = end;
		if (start != end)
			configs.push_back(std::make_pair(str.substr(start, end - start), indent));
	}
	return configs;
}

std::vector<std::vector<std::pair<std::string, int> > > findChunck(std::vector<std::pair<std::string, int> > indents, std::string keyword)
{
	std::vector<std::vector<std::pair<std::string, int> > > multiChunck;
	size_t start = 0;
	size_t	end = 0;
	
	while (end < indents.size())
	{
		while (start < indents.size() && nth_word(indents[start].first, 1) != keyword)
			++ start;
		end = start;
		while (end + 1 < indents.size() && indents[end + 1].second > indents[start].second)
			++ end;
		std::vector<std::pair<std::string, int> > chunck(indents.begin() + start, indents.begin() + end + 1);
		multiChunck.push_back(chunck);
		if (end == indents.size())
			break;
		++ end;
		start = end;
	}
	return multiChunck;
}

std::string	parseString(const std::vector<std::pair<std::string, int> > chunck, std::string keyword)
{
	size_t	i = 0;

	while (i < chunck.size() && nth_word(chunck[i].first, 1) != keyword)
		i ++;
	if (i == chunck.size())
		return "";
	return nth_word(chunck[i].first, 2);
}

std::map<int, std::string> parseErrorPages(const std::vector<std::pair<std::string, int> > chunck)
{
	std::map<int, std::string> error_pages;
	std::vector<std::pair<std::string, int> > errorChunck;
	std::string keyword = "errorPages";

	errorChunck = findChunck(chunck, keyword)[0];
	for (size_t i = 1; i < errorChunck.size(); i ++)
		error_pages[std::atoi(nth_word(errorChunck[i].first, 1).c_str())] = nth_word(errorChunck[i].first, 2);
	return error_pages;
}

t_server parseServerConfig(const std::vector<std::pair<std::string, int> > chunck) {

	t_server server;

	server.port = parseString(chunck, "port");
	server.host = parseString(chunck, "host");
	server.server_name = parseString(chunck, "server_name");
	server.client_max_body_size = std::atoi(parseString(chunck, "client_max_body_size").c_str());
	server.timeoutTime = std::atoi(parseString(chunck, "timeoutTime").c_str());
	server.timeoutReads = std::atoi(parseString(chunck, "timeoutReads").c_str());
	server.error_pages = parseErrorPages(chunck);
	server.locations = parseLocations(chunck);
	return server;
}

std::vector <t_server> configParse(std::string configFilePath)
{
	std::string									config;
	std::vector <t_server>						servers;
	std::vector<std::pair<std::string, int> >	indents;
	std::vector<std::vector<std::pair<std::string, int> > > chuncks;

	try{
		config = openFile(configFilePath);
	} catch (const std::runtime_error &e){
		std::cerr << "Caught execption: " << e.what() << '\n';
		return std::vector<t_server>();
	}
	indents = findIndent(config);
	chuncks = findChunck(indents, "server");
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

int main() {
	std::string configFilePath = "parsers/sampleConfig.conf";
	std::vector<t_server> servers = configParse(configFilePath);
	
	if (servers.empty()) {
		std::cout << "Failed to parse the configuration file." << std::endl;
		return 1;
	}
	
	// Print the parsed server configurations
	for (const auto& server : servers) {
		std::cout << "Server Configuration:" << std::endl;
		std::cout << "Port: " << server.port << std::endl;
		std::cout << "Host: " << server.host << std::endl;
		std::cout << "Server Name: " << server.server_name << std::endl;
		std::cout << "Client Max Body Size: " << server.client_max_body_size << std::endl;
		std::cout << "Timeout Time: " << server.timeoutTime << std::endl;
		std::cout << "Timeout Reads: " << server.timeoutReads << std::endl;
		
		std::cout << "Error Pages:" << std::endl;
		for (const auto& errorPage : server.error_pages) {
			std::cout << "Error Code: " << errorPage.first << ", Page: " << errorPage.second << std::endl;
		}
		
		std::cout << "Locations:" << std::endl;
		for (const auto& location : server.locations) {
			//std::cout << "Location Path: " << location.path << std::endl;
			std::cout << "Location Root: " << location.second._root << std::endl;
			std::cout << "Location Index: " << location.second._index << std::endl;
			std::cout << "Location Methods: ";
			std::cout << location.second._httpMethods << std::endl;
			std::cout << std::endl;
		}
		
		std::cout << std::endl;
	}
	
	return 0;
}