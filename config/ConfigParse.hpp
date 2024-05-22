/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: siun <siun@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 22:37:54 by siun              #+#    #+#             */
/*   Updated: 2024/05/22 17:29:57 by siun             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSE_HPP
# define CONFIGPARSE_HPP
# include <string>
# include <cstring>
# include <exception>
# include <vector>
# include <iostream>
# include <fstream>
# include <iterator>
# include <exception>
# include <algorithm>
# include <map>

class ConfigParse
{
	private:
		
};


//going to saved as a vector to save multiple servers inside of one config file
typedef struct s_server
{
	std::string	 	port = NULL;
	std::string 	host = NULL;
	std::string		server_name = NULL;
	std::string 	default_error_page = NULL;
	int 			client_max_body_size = 0;
	std::vector		<std::string> httpMethods = std::vector<std::string>();
	std::string		httpRedirection = NULL;
	std::vector		<std::string> path = std::vector<std::string>();
	std::map 		<std::string, std::string> cgi_extension = std::map<std::string, std::string>();
	bool			dir_listing = false;
	std::string		dir_request_default = NULL; //default file if the request is a directory
}t_server;

std::string openFile(std::string path);
std::vector<std::pair<std::string, int>>	findIndent(std::string str);
std::vector<std::string> findChunck(std::string config, std::vector<int> indents);
std::vector<std::string> findMethods(std::string methods);
std::vector<std::string> findPaths(std::string paths);
std::map<std::string, std::string> findCgi(std::string cgi);
std::string findAndExtractValue(const std::string& chunck, const std::string& keyword);
t_server parseServerConfig(const std::string& chunck);

std::vector <t_server> configParse(std::string configFilePath);

# endif