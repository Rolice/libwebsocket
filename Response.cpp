#include "Response.hpp"

namespace ws
{

Response::Response()
{
	m_length = 0;
}

Response::Response(const char *content) : HttpMessage(content)
{
	if(m_control_line.empty())
		return;

	size_t last_pos = std::string::npos;
	size_t pos = m_control_line.find(" ");

	if(std::string::npos == pos)
		return;

	m_version = m_control_line.substr(0, pos);

	last_pos = pos + 1;
	pos = m_control_line.find(" ", last_pos);

	if(std::string::npos == pos)
		return;

	std::stringstream stream(m_control_line.substr(last_pos, pos - last_pos));

	stream >> m_status;

	last_pos = pos + 1;
	pos = m_control_line.find(" ", last_pos);

	if(std::string::npos == pos)
		return;

	m_reason = m_control_line.substr(last_pos, pos - last_pos);
}

Response::~Response()
{
}

}