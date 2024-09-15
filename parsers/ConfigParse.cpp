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
	std::vector<std::pair<std::string, int> >::iterator start = indents.begin();
	std::vector<std::pair<std::string, int> >::iterator end;

	while (start < indents.end()){
		if (nth_word(start->first, 1) == keyword){
			end = start + 1;
			while (end < indents.end() && end->second > start->second){
				++ end;
			}
			if (end < indents.end())
				multiChunck.push_back(std::vector<std::pair<std::string, int> >(start, end));
			else
				multiChunck.push_back(std::vector<std::pair<std::string, int> >(start, indents.end()));
			start = end;
		}
		else
			++ start;
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

std::map<statusCode, std::string> parseErrorPages(const std::vector<std::pair<std::string, int> > chunck)
{
	std::map<statusCode, std::string> error_pages;
	for (size_t i = 0; i < chunck.size(); i ++)
	{
		if (nth_word(chunck[i].first, 1) == "errorPages")
		{
			int indent = chunck[i].second;
			i ++;
			while (i < chunck.size() && chunck[i].second > indent)
			{
				statusCode error_code = (statusCode)std::atoi(nth_word(chunck[i].first, 1).c_str());
				std::string page = nth_word(chunck[i].first, 2);
				error_pages.insert(std::make_pair(error_code, page));
				++ i ;
			}
		}
	}
	return error_pages;
}

t_server parseServerConfig(const std::vector<std::pair<std::string, int> > chunck) {

	t_server server;

	server.port = parseString(chunck, "port");
	server.host = parseString(chunck, "host");
	server.serverName = parseString(chunck, "server_name");
	server.clientMaxBodySize = std::atoi(parseString(chunck, "client_max_body_size").c_str());
	server.timeoutTime = std::atoi(parseString(chunck, "timeoutTime").c_str());
	server.timeoutReads = std::atoi(parseString(chunck, "timeoutReads").c_str());
	server.errorPages = parseErrorPages(chunck);
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
}
