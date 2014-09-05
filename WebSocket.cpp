#include "WebSocket.hpp"

namespace ws
{

WebSocket::WebSocket()
{
	m_running = false;
	m_server = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

	if(0 > m_server)
	{
		Debug::Error("Could not create socket.");
		exit(ERR_NETWORK);
	}

	int opt = YES;

	if(0 > setsockopt(m_server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)))
	{
		Debug::Error("Socket reuse setup failed.", errno);
		exit(ERR_NETWORK);
	}

	if(0 > setsockopt(m_server, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(int)))
	{
		Debug::Error("Socket keep alive setup failed.", errno);
		exit(ERR_NETWORK);
	}

	struct sockaddr_in server_addr;

	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(SOCK_PORT);

	int binding = bind(m_server, (struct sockaddr *) &server_addr, sizeof(server_addr));

	if(0 > binding)
	{
		Debug::Error("Could not bind socket.");
		return;
	}

	m_server_addr = &server_addr;

	m_running = true;
}

WebSocket::WebSocket(int server, struct sockaddr_in *server_addr, int client, struct sockaddr_in *client_addr)
{
	m_running = false;

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

	Debug::Info("Handshake received.");

	m_running = true;
}

WebSocket::~WebSocket()
{
	m_running = false;
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

	key += RFC6455_KEY_GUID;                               // RFC 6544 Magic GUID

	unsigned char digest[20];                              // 160-bit SHA1 hash container
	SHA1 sha;

	sha.Input(key.data(), key.size());
	sha.Result((unsigned *) digest);

	// Little to Big Endian
	for(unsigned int i = 0; i < sizeof(digest); i += 4)
	{
		digest[i]        ^= digest[i + 3];
		digest[i + 3]    ^= digest[i];
		digest[i]        ^= digest[i + 3];

		digest[i + 1]    ^= digest[i + 2];
		digest[i + 2]    ^= digest[i + 1];
		digest[i + 1]    ^= digest[i + 2];
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

void WebSocket::ParseHandshake()
{
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
}

void WebSocket::Listen()
{
	int in = 0;
	int max = 65535;
	char buffer[max];

	while(m_running && (in = recv(m_client, &buffer, max , 0)))
	{
		std::string message(buffer);
		Process(message);
	}
}

void WebSocket::Listen(bool x)
{
	int client_len = sizeof(struct sockaddr_in);

	listen(m_server, SOCK_BACKLOG);

	while(m_running && 0 <= (m_client = accept(m_server, (struct sockaddr *) m_client_addr, (socklen_t *) &client_len)))
	{
		ParseHandshake();
		Handshake();

		int in = 0;
		int buffer_length = 512;
		char buffer[buffer_length];

		while(0 != (in = recv(m_client, &buffer, buffer_length, 0)))
		{
			char *end;
			std::string reason;
			struct Frame frame;
			Frame::ParseResult result = Frame::Parse(buffer, sizeof(buffer), frame, end, reason); 

			memset(&buffer, 0, sizeof(buffer));

			Debug::Warn(frame.payload);

			if(NULL == frame.payload)
				continue;

			std::string message(frame.payload);
			Process(message);
		}

		Debug::Warn("Receiving ended.");
	}

	Debug::Warn("Connection acception ended.");
}

void WebSocket::Process(std::string message)
{
	std::cout << message << std::endl << std::endl;
}

void WebSocket::Stop()
{
	m_running = false;
}

}