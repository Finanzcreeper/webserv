/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigLocationParse.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: subpark <subpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:43:52 by subpark           #+#    #+#             */
/*   Updated: 2024/06/06 14:21:07 by subpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../CommonIncludes.hpp"
#include "ConfigParse.hpp"


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

std::vector<std::string>	parseIndex(const std::vector<std::pair<std::string, int> > chunck)
{
	std::vector<std::string> indexs;
	std::string				keyword = "index";
	size_t	i = 0;

	while (i < chunck.size() && chunck[i].first.find(keyword) == std::string::npos) {
		++i;
	}
	int	indent = chunck[i].second;
	i++;
	while (i < chunck.size() && chunck[i].second == indent + 1) {
		std::istringstream iss(chunck[i].first);
		std::string index;
		iss >> index;
		indexs.push_back(index);
		++i;
	}
	return indexs;
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

location::location(std::vector<std::pair<std::string, int> > location, std::string prefix)
{
	_prefix = prefix;
	_httpMethods = parseMethod(location);
}

index::index(std::vector<std::pair<std::string, int> > location, std::string prefix):location(location, prefix)
{
	_index = parseIndex(location);
}

cgi::cgi(std::vector<std::pair<std::string, int> > location, std::string prefix):location(location, prefix)
{
	cgi_extension = parseCgi(location);
}

std::pair<location*, int>createLocation(std::vector<std::pair<std::string, int> > lChunck)
{
	location *tmp = new location(lChunck, "/");
	return (std::pair<location*, int>(tmp, tmp->getMethods()));
}

std::pair<location*, int>createIndex(std::vector<std::pair<std::string, int> > lChunck)
{
	location *tmp = new index(lChunck, "/index");
	return (std::pair<location*, int>(tmp, tmp->getMethods()));
}

std::pair<location*, int>createCgi(std::vector<std::pair<std::string, int> > lChunck)
{
	location *tmp = new cgi(lChunck, "/cgi");
	return (std::pair<location*, int>(tmp, tmp->getMethods()));
}

std::map<location*, int> parseLocations(const std::vector<std::pair<std::string, int> > chunck)
{
	std::vector<std::vector<std::pair<std::string, int> > > locationChuncks;

	locationChuncks = findChunck(chunck, "locations");
	std::string	types[3] = {"/", "/index", "/cgi"};
	std::pair<location*, int>	(creates[3])(std::vector<std::pair<std::string, int> >) = {createLocation, createIndex, createCgi};
	for (int i = 0; i < 3; i ++)
	{
		std::string tmp = parseString(locationChuncks[i], "location");
		if (tmp == types[i])

	}
	
}