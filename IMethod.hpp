#ifndef AMETHOD_HPP
#define AMETHOD_HPP

#include <iostream>

typedef enum e_method_type{
    TYPE_GET,
    TYPE_POST,
    TYPE_DELETE,
    TYPE_HEAD,
    TYPE_ABSTRACT
}   t_method_type;

class IMethod
{
    private:
        t_method_type   _type;
        int             _status_code;
        std::ostream    _response;
        
        //Envrinoment object *   configuration;
    public:
        virtual void execute() = 0;
        virtual ~IMethod() = 0;
};

IMethod * create_request()
{
    IMethod *request;
    //switch (request_type)
}
#endif