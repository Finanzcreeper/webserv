#include "server/Server.hpp"
#include <vector>
#include "parsers/ConfigParse.hpp"

/*
int main() {
	//std::vector<t_server> configs;
	//std::vector<t_server>::iterator servernbr = configs.begin();
	std::vector<t_server> Config;
	try  {
		Config = configParse("/home/nreher/dev/webserv/parsers/sampleConfig.conf");
	} catch (std::runtime_error &e) {
		std::cout <<e.what() << std::endl;
	}
		Server *sock = new Server(Config[0]);
		sock->CheckForConnections();
		//servernbr++;
	//}

	delete sock;
}
 */

#include "tests/Tests.hpp"

int main() {
	std::cout << "Running tests: " << std::endl;

	Tests tester;

	tester.testing();
}
