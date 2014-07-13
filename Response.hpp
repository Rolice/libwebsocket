#ifndef __RESPONSE_HPP__
#define __RESPONSE_HPP__

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

class Response : public HttpMessage
{

protected:
	std::string                        m_version;
	unsigned int                       m_status;
	std::string                        m_reason;

public:
	Response();
	Response(const char *content);
	virtual ~Response();

};

}

#endif