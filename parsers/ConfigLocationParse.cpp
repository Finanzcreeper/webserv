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
		loc.locationName = tmp;
		loc.httpMethods = parseMethod(locationChuncks[i]);
		loc.dirListing = !strcmp("ON", parseString(locationChuncks[i], "dirlisting").c_str());
		loc.index = parseString(locationChuncks[i], "index");
		loc.redirect = parseString(locationChuncks[i], "redirect");
		loc.cgi = parseCgi(locationChuncks[i]);
		loc.root = parseString(locationChuncks[i], "root");
		locations.insert(std::pair<std::string, location>(tmp, loc));
	}
	return locations;
}
