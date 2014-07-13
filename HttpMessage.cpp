#include "HttpMessage.hpp"

namespace ws
{

HttpMessage::HttpMessage()
{
}

HttpMessage::HttpMessage(const char *content)
{
	m_valid = false;

	if(!content)
		return;

	std::string str = content;
	std::string sep = HTTP_HEADER_SEPARATOR;
	size_t pos = str.find(sep);

	m_length = str.length();
	m_control_line = str.substr(0, pos);

	std::string headers = str.substr(pos + sep.length(), str.length() - pos - sep.length());

	sep = HTTP_COMPONENT_SEPARATOR;
	pos = headers.find(sep);

	if(std::string::npos == pos)
	{
		Debug::Warn("Ignoring invalid request.");
		return;
	}

	m_body = headers.substr(pos + sep.length(), headers.length() - pos - sep.length());
	headers = headers.substr(0, pos);

	std::size_t last_pos = 0;

	do
	{
		if(headers.length() <= last_pos + 1)
			break;

		sep = HTTP_HEADER_SEPARATOR;
		pos = headers.find(sep, last_pos + 1);

		if(std::string::npos == pos)
			pos = headers.length();

		std::string header = headers.substr(last_pos, pos - last_pos);
		std::string name, value;

		last_pos = pos + sep.length();

		std::size_t inner_pos = header.find(":");

		if(std::string::npos == inner_pos)
		{
			std::string message = "Ignoring invalid header: ";
			message = message + header;

			Debug::Warn(message.c_str());
			continue;
		}
		
		name = trim(header.substr(0, inner_pos));
		value = trim(header.substr(1 + inner_pos, header.length() - inner_pos - 1));

		m_headers.insert(Header(name, value));
	}
	while(std::string::npos != last_pos);

	// for(HeaderCollectionIterator it = m_headers.begin(); it != m_headers.end(); ++it)
	// 	std::cout << it->first << "<=>" << it->second << std::endl;

	// std::cout << "Total: " << m_headers.size() << std::endl;

	m_valid = true;
}

HttpMessage::~HttpMessage()
{
}

std::string HttpMessage::GetHeader(std::string key)
{
	std::string result = "";

	if(!m_headers.count(key))
		return result;

	return m_headers.find(key)->second;
}

unsigned int HttpMessage::Length()
{
	return m_length;
}

std::string HttpMessage::ToString()
{
	std::string result = m_control_line + "\r\n";

	for(HeaderCollectionIterator it = m_headers.begin(); it != m_headers.end(); ++it)
		result += it->first + ": " + it->second + "\r\n";

	result += "\r\n";
	result += m_body;

	return result;
}

}