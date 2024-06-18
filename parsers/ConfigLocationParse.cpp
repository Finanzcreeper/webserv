/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigLocationParse.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: siun <siun@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:43:52 by subpark           #+#    #+#             */
/*   Updated: 2024/06/17 23:43:54 by siun             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../CommonIncludes.hpp"
#include "ConfigParse.hpp"


int parseMethod(const std::vector<std::pair<std::string, int> >& chunck) {
	int methods = 0;
	std::string str;
	std::string method;

	str = parseString(chunck,"method");
	std::istringstream stream(str);
	while(stream >> method) {
		if (method == "GET") {
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
		}else {
			methods = methods | INVALID;
		}
	}
	return (methods);
}

std::map<std::string, std::string> parseCgi(std::vector<std::pair<std::string, int> > chunck)
{
	std::map<std::string, std::string> cgi;
	std::string keyword = "cgi";
	size_t	i = 0;

	while (i < chunck.size() && nth_word(chunck[i].first, 1) != keyword)
		++i;
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

std::map<std::string, location> parseLocations(std::vector<std::pair<std::string, int> > chunck)
{
	std::map<std::string, location>	locations;
	location						loc;
	std::vector<std::vector<std::pair<std::string, int> > > locationChuncks;

	locationChuncks = findChunck(chunck, "location");

	// for (int i = 0; i < locationChuncks.size(); i++)
	// {
	// 	std::cout << "Location Chunk " << i << ":" << std::endl;
	// 	for (const auto& pair : locationChuncks[i])
	// 	{
	// 		std::cout << pair.first << " : " << pair.second << std::endl;
	// 	}
	// 	std::cout << std::endl;
	// }
	
	for (int i = 0; i < locationChuncks.size(); i ++)
	{
		std::string tmp = parseString(locationChuncks[i], "location");
		loc._httpMethods = parseMethod(locationChuncks[i]);
		std::cout << "dirlisting: " << parseString(locationChuncks[i], "dirlisting") << std::endl;
		loc._dir_listing = !strcmp("ON", parseString(locationChuncks[i], "dirlisting").c_str());
		loc._errorPage = parseString(locationChuncks[i], "errorPage");
		loc._index = parseString(locationChuncks[i], "index");
		loc._redirect = parseString(locationChuncks[i], "redirect");
		loc._cgi = parseCgi(locationChuncks[i]);
		loc._path = parseString(locationChuncks[i],  "path");
		locations.insert(std::pair<std::string, location>(tmp, loc));
	}
	return locations;
}
