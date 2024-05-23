/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: siun <siun@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 22:37:54 by siun              #+#    #+#             */
/*   Updated: 2024/05/23 16:46:20 by siun             ###   ########.fr       */
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
	std::string	 	port = "";
	std::string 	host = "";
	std::string		server_name = "";
	std::string 	default_error_page = "";
	int 			client_max_body_size = 0;
	std::vector		<std::string> httpMethods = std::vector<std::string>();
	std::string		httpRedirection = "";
	std::vector		<std::string> path = std::vector<std::string>();
	std::map 		<std::string, std::string> cgi_extension = std::map<std::string, std::string>();
	bool			dir_listing = false;
	std::string		dir_request_default = ""; //default file if the request is a directory
}t_server;

std::string openFile(std::string path);
std::vector<std::pair<std::string, int>>	findIndent(std::string str);
std::vector<std::vector<std::pair<std::string, int>>> findChunck(std::vector<std::pair<std::string, int>> indents);
t_server parseServerConfig(const std::vector<std::pair<std::string, int>> chunck);

std::vector <t_server> configParse(std::string configFilePath);

# endif