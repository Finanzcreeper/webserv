#include "statusCodes.h"

std::string getStatusCodeMessage(statusCode code){
    switch (code)
    {
        case (CONTINUE):
            return ("Continue");
		case (SWITCHING_PROTOCOLS):
            return ("Switching Protocols");
		case (PROCESSING):
            return ("Processing");
		case (EARLY_HINTS):
            return ("Early Hints");
		case (OK_HTTP):
            return ("OK");
		case (CREATED):
            return ("Created");
		case (ACCEPTED):
            return ("Accepted");
		case (NON_AUTHORITATIVE_INFORMATION):
            return ("Non-Authoritative Information");
		case (NO_CONTENT):
            return ("No Content");
		case (RESET_CONTENT):
            return ("Reset Content");
		case (PARTIAL_CONTENT):
            return ("Partial Content");
		case (MULTIPLE_CHOICES):
            return ("Multiple Choices");
		case (MOVED_PERMANENTLY):
            return ("Moved Permanently");
		case (FOUND):
            return ("Found");
		case (SEE_OTHER):
            return ("See Other");
		case (NOT_MODIFIED):
            return ("Not Modified");
		case (TEMPORARY_REDIRECT):
            return ("Temporary Redirect");
		case (PERMANENT_REDIRECT):
            return ("Permanent Redirect");
		case (BAD_REQUEST):
            return ("Bad Request");
		case (UNAUTHORIZED):
            return ("Unauthorized");
		case (FORBIDDEN):
            return ("Forbidden");
		case (NOT_FOUND):
            return ("Not Found");
		case (METHOD_NOT_ALLOWED):
            return ("Method Not Allowed");
		case (NOT_ACCEPTABLE):
            return ("Not Acceptable");
		case (PROXY_AUTHENTIFICATION_REQUIRED):
            return ("Proxy Authentication Required");
		case (REQUEST_TIMEOUT):
            return ("Request Timeout");
		case (CONFILCT):
            return ("Conflict");
		case (GONE):
            return ("Gone");
		case (LENGTH_REQUIRED):
            return ("Length Required");
		case (PRECONDITION_FAILED):
            return ("Precondition Failed");
		case (PAYLOAD_TO_LARGE):
            return ("Content Too Large");
		case (URI_TOO_LONG):
            return ("Request-URI Too Long");
		case (UNSUPPORTED_MEDIA_TYPE):
            return ("Unsupported Media Type");
		case (RANGE_NOT_SATISFIABLE):
            return ("Requested Range Not Satisfiable");
		case (EXPECTATION_FAILED):
            return ("Expectation Failed");
		case (IM_A_TEAPOT):
            return ("I'm a teapot");
		case (MISDIRECTED_REQUEST):
            return ("Misdirected Request");
		case (UNPROCESSABLE_CONTENT):
            return ("Unprocessable Content");
		case (UPGRADE_REQUIRED):
            return ("Upgrade Required");
		case (INTERNAL_SERVER_ERROR):
            return ("Internal Server Error");
		case (NOT_IMPLEMENTED):
            return ("Not Implemented");
		case (BAD_GATEWAY):
            return ("Bad Gateway");
		case (SERVICE_UNAVAILABLE):
            return ("Service Unavailable");
		case (GATEWAY_TIMEOUT):
            return ("Gateway Timeout");
		case (HTTP_VERSION_NOT_SUPPORTED):
            return ("HTTP Version Not Supported");
		default:
			return ("UNDEFINED HTTP STATUS CODE");
    }
}

//TODO: Insert more detailed error descriptions
std::string getStatusCodeDescription(statusCode code){
	switch (code)
    {
        case (CONTINUE):
            return ("Continue");
		case (SWITCHING_PROTOCOLS):
            return ("Switching Protocols");
		case (PROCESSING):
            return ("Processing");
		case (EARLY_HINTS):
            return ("Early Hints");
		case (OK_HTTP):
            return ("OK");
		case (CREATED):
            return ("Created");
		case (ACCEPTED):
            return ("Accepted");
		case (NON_AUTHORITATIVE_INFORMATION):
            return ("Non-Authoritative Information");
		case (NO_CONTENT):
            return ("No Content");
		case (RESET_CONTENT):
            return ("Reset Content");
		case (PARTIAL_CONTENT):
            return ("Partial Content");
		case (MULTIPLE_CHOICES):
            return ("Multiple Choices");
		case (MOVED_PERMANENTLY):
            return ("Moved Permanently");
		case (FOUND):
            return ("Found");
		case (SEE_OTHER):
            return ("See Other");
		case (NOT_MODIFIED):
            return ("Not Modified");
		case (TEMPORARY_REDIRECT):
            return ("Temporary Redirect");
		case (PERMANENT_REDIRECT):
            return ("Permanent Redirect");
		case (BAD_REQUEST):
            return ("Bad Request");
		case (UNAUTHORIZED):
            return ("Unauthorized");
		case (FORBIDDEN):
            return ("Forbidden");
		case (NOT_FOUND):
            return ("Not Found");
		case (METHOD_NOT_ALLOWED):
            return ("Method not allowed or not supported yet");
		case (NOT_ACCEPTABLE):
            return ("Not Acceptable");
		case (PROXY_AUTHENTIFICATION_REQUIRED):
            return ("Proxy Authentication Required");
		case (REQUEST_TIMEOUT):
            return ("Request Timeout");
		case (CONFILCT):
            return ("Conflict");
		case (GONE):
            return ("Gone");
		case (LENGTH_REQUIRED):
            return ("Length Required");
		case (PRECONDITION_FAILED):
            return ("Precondition Failed");
		case (PAYLOAD_TO_LARGE):
            return ("Content Too Large");
		case (URI_TOO_LONG):
            return ("Request-URI Too Long");
		case (UNSUPPORTED_MEDIA_TYPE):
            return ("Unsupported Media Type");
		case (RANGE_NOT_SATISFIABLE):
            return ("Requested Range Not Satisfiable");
		case (EXPECTATION_FAILED):
            return ("Expectation Failed");
		case (IM_A_TEAPOT):
            return ("I'm a teapot");
		case (MISDIRECTED_REQUEST):
            return ("Misdirected Request");
		case (UNPROCESSABLE_CONTENT):
            return ("Unprocessable Content");
		case (UPGRADE_REQUIRED):
            return ("Upgrade Required");
		case (INTERNAL_SERVER_ERROR):
            return ("Internal Server Error");
		case (NOT_IMPLEMENTED):
            return ("Not Implemented");
		case (BAD_GATEWAY):
            return ("Bad Gateway");
		case (SERVICE_UNAVAILABLE):
            return ("Service Unavailable");
		case (GATEWAY_TIMEOUT):
            return ("Gateway Timeout");
		case (HTTP_VERSION_NOT_SUPPORTED):
            return ("HTTP Version Not Supported");
		default:
			return ("UNDEFINED HTTP STATUS CODE");
    }
}
