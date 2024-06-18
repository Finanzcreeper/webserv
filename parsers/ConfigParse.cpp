/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: siun <siun@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 22:37:44 by siun              #+#    #+#             */
/*   Updated: 2024/06/18 15:40:01 by siun             ###   ########.fr       */
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

	// for (size_t i = 0; i < multiChunck.size(); i++)
	// {
	// 	std::cout << "Chunk " << i << ":\n";
	// 	for (size_t j = 0; j < multiChunck[i].size(); j++)
	// 	{
	// 		std::cout << "Indent: " << multiChunck[i][j].second << std::endl;
	// 		std::cout << "Content: " << multiChunck[i][j].first << std::endl;
	// 		std::cout << "------------------------\n";
	// 	}
	// 	std::cout << "=========================================\n";
	// }
	return multiChunck;
}
//last line of the each chunck is not added...HAVE TO FIX

std::string	parseString(const std::vector<std::pair<std::string, int> > chunck, std::string keyword)
{
	size_t	i = 0;

	while (i < chunck.size() && nth_word(chunck[i].first, 1) != keyword)
		i ++;
	if (i == chunck.size())
		return "";
	return nth_word(chunck[i].first, 2);
}

t_server parseServerConfig(const std::vector<std::pair<std::string, int> > chunck) {

	t_server server;

	server.port = parseString(chunck, "port");
	server.host = parseString(chunck, "host");
	server.server_name = parseString(chunck, "server_name");
	server.default_error_page = parseString(chunck, "default_error_page");
	server.client_max_body_size = std::atoi(parseString(chunck, "client_max_body_size").c_str());
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

int main()
{
	std::vector<t_server> servers = configParse("parsers/sampleConfig.conf");
	for (size_t i = 0; i < servers.size(); i ++)
	{
		std::cout << "Server " << i << ":\n";
		std::cout << "port: " << servers[i].port << std::endl;
		std::cout << "host: " << servers[i].host << std::endl;
		std::cout << "server_name: " << servers[i].server_name << std::endl;
		std::cout << "default_error_page: " << servers[i].default_error_page << std::endl;
		std::cout << "client_max_body_size: " << servers[i].client_max_body_size << std::endl;
		for (std::map<std::string, location>::iterator it = servers[i].locations.begin(); it != servers[i].locations.end(); it ++)
		{
			std::cout << "--------------------------------------------------\n";
			std::cout << "location: " << it->first << std::endl;
			std::cout << "dir_listing: " << it->second._dir_listing << std::endl;
			std::cout << "httpMethods: " << it->second._httpMethods << std::endl;
			std::cout << "index: " << it->second._index << std::endl;
			std::cout << "cgi:\n";
			for (std::map<std::string, std::string>::iterator it2 = it->second._cgi.begin(); it2 != it->second._cgi.end(); it2 ++)
			{
				std::cout << it2->first << " " << it2->second << std::endl;
			}
			std::cout << "redirect:  "<< it->second._redirect << std::endl;
		}
							std::cout << "=========================================================\n";

	}
	return 0;
}