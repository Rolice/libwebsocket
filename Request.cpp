#include "Request.hpp"

namespace ws
{

Request::Request()
{
	m_valid = false;
	m_length = 0;
}

Request::Request(const char *content) : HttpMessage(content)
{
	if(m_control_line.empty())
		return;

	size_t last_pos = std::string::npos;
	size_t pos = m_control_line.find(" ");

	if(std::string::npos == pos)
		return;

	m_method = m_control_line.substr(0, pos);

	last_pos = pos + 1;
	pos = m_control_line.find(" ", last_pos);

	if(std::string::npos == pos)
		return;

	m_uri = m_control_line.substr(last_pos, pos - last_pos);

	last_pos = pos + 1;
	pos = m_control_line.find(" ", last_pos);

	if(std::string::npos == pos)
		return;

	m_version = m_control_line.substr(last_pos, pos - last_pos);
}

Request::~Request()
{
}

}