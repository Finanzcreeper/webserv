/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigLocationParse.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: siun <siun@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:43:52 by subpark           #+#    #+#             */
/*   Updated: 2024/06/18 16:33:34 by siun             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../CommonIncludes.hpp"
#include "ConfigParse.hpp"


int parseMethod(const std::vector<std::pair<std::string, int> >& chunck) {
	int methods = 0;
	std::string str;
	std::string method;

	int i = 0;
	while (i < chunck.size() && nth_word(chunck[i].first, 1) != "method")
		++i;
	str = chunck[i].first;
	std::cout << "str: " << str << std::endl;
	std::istringstream stream(str);
	stream >> method;
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
		} else {
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
	if (i != chunck.size())
		cgi.insert(std::pair<std::string, std::string>(nth_word(chunck[i].first, 2), nth_word(chunck[i].first, 3)));
	return cgi;
}

std::map<std::string, location> parseLocations(std::vector<std::pair<std::string, int> > chunck)
{
	std::map<std::string, location>	locations;
	location						loc;
	std::vector<std::vector<std::pair<std::string, int> > > locationChuncks;

	locationChuncks = findChunck(chunck, "location");
	for (int i = 0; i < locationChuncks.size(); i ++)
	{
		std::string tmp = parseString(locationChuncks[i], "location");
		loc._httpMethods = parseMethod(locationChuncks[i]);
		loc._dir_listing = !strcmp("ON", parseString(locationChuncks[i], "dirlisting").c_str());
		loc._errorPage = parseString(locationChuncks[i], "errorPage");
		loc._index = parseString(locationChuncks[i], "index");
		loc._redirect = parseString(locationChuncks[i], "redirect");
		loc._cgi = parseCgi(locationChuncks[i]);
		loc._path = parseString(locationChuncks[i],  "path");
		loc._root = parseString(locationChuncks[i], "root");
		locations.insert(std::pair<std::string, location>(tmp, loc));
	}
	return locations;
}
