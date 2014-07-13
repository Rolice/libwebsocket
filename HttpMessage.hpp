#ifndef __HTTP_MESSAGE_HPP__
#define __HTTP_MESSAGE_HPP__

#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <sstream>

#include "include/http.hpp"
#include "include/string.hpp"

#include "Debug.hpp"

namespace ws
{

class HttpMessage
{

protected:
	std::string m_control_line;

	bool m_valid;
	unsigned int m_length;
	std::string m_body;

	HeaderCollection m_headers;

public:
	HttpMessage();
	HttpMessage(const char *content);
	virtual ~HttpMessage();

	virtual std::string GetHeader(std::string key);
	virtual unsigned int Length();
	virtual std::string ToString();

};

}

#endif