#ifndef __REQUEST_HPP__
#define __REQUEST_HPP__

#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <sstream>

#include "include/http.hpp"
#include "include/string.hpp"

#include "Debug.hpp"
#include "HttpMessage.hpp"

namespace ws
{

class Request : public HttpMessage {

protected:
	std::string                        m_method;
	std::string                        m_uri;
	std::string                        m_version;

public:
	Request();
	Request(const char *content);
	virtual ~Request();
	
};

}

#endif