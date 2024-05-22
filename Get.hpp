#ifndef GET_HPP
#define GET_HPP

#include "IMethod.hpp"
#include <unistd.h>
#include <string>

class Get: public IMethod
{
private:
	e_method_type	_type = TYPE_GET;
	std::string		_filename;
public:
	Get(/* args */);
	~Get();
	void execute( void );
};

Get::Get(/* args */)
{
}

Get::~Get()
{
}

void    Get::execute( void )
{
	std::string root = "/home/thofting/repos/webserv_creeper/content"
	if (access(root.append()))
}

#endif