#include "WebSocket.hpp"

WebSocket::WebSocket(int server, struct sockaddr_in *server_addr, int client, struct sockaddr_in *client_addr)
{
	m_server = server;
	m_client = client;

	m_server_addr = server_addr;
	m_client_addr = client_addr;

	char request[65535];
	int request_length = 0;

	memset(request, 0, sizeof(request));

	request_length = read(m_client, request, sizeof(request));

	if(0 >= request_length)
	{
		Debug::Warn("Request with no data ignored.");
		return;
	}

	Request *req = new Request(request);

	m_host = req->GetHeader("Host");
	m_origin = req->GetHeader("Origin");
	m_protocol = req->GetHeader("Sec-WebSocket-Protocol");
	m_key = req->GetHeader("Sec-WebSocket-Key");
	m_accept = AcceptKey();

	Debug::Info(sizeof(Frame));

	Debug::Info("Handshake received.");
}

WebSocket::~WebSocket()
{
}

std::string WebSocket::AcceptKey()
{

	return AcceptKey(m_key);
}

std::string WebSocket::AcceptKey(std::string key)
{
	if(key.empty())
		key = m_key;

	std::string result = "";

	if(key.empty())
		return result;

	key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";							//RFC6544_MAGIC_KEY

	unsigned char digest[20];												// 160-bit SHA1 digest
	SHA1 sha;

	sha.Input(key.data(), key.size());
	sha.Result((unsigned *) digest);

	// Little to Big Endian
	for(unsigned int i = 0; i < sizeof(digest); i += 4)
	{
		digest[i]		^= digest[i + 3];
		digest[i + 3]	^= digest[i];
		digest[i]		^= digest[i + 3];

		digest[i + 1]	^= digest[i + 2];
		digest[i + 2]	^= digest[i + 1];
		digest[i + 1]	^= digest[i + 2];
	}

	result = base64_encode((const unsigned char *) digest, sizeof(digest));

	return result;
}

void WebSocket::Handshake()
{
	std::string content = "";

	content += "HTTP/1.1 101 Switching Protocols\r\n";
	content += "Upgrade: WebSocket\r\n";
	content += "Connection: Upgrade\r\n";

	if(0 < m_key.length())
		content += "Sec-WebSocket-Accept: " + m_accept + "\r\n";

	if(0 < m_protocol.length())
		content += "Sec-WebSocket-Protocol: " + m_protocol + "\r\n";

	content += "\r\n";

	Response *res = new Response(content.c_str());

	write(m_client, res->ToString().c_str(), res->Length());

	Debug::Info("Handshake accepted.");
}

void WebSocket::Listen()
{
	int in = 0;
	int max = 65535;
	char buffer[max];

	while((in = recv(m_client, &buffer, max , 0)))
	{
		std::string message(buffer);
		Process(message);
	}
}

void WebSocket::Process(std::string message)
{
	std::cout << message << std::endl << std::endl;
}