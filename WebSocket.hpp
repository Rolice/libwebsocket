#ifndef __WEBSOCKET_HPP__
#define __WEBSOCKET_HPP__

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "include/base64/base64.h"
#include "include/sha1/sha1.h"

#include "Response.hpp"
#include "Request.hpp"

#define RFC6455_KEY_GUID               "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"  // RFC 6455 Magic GUID

class WebSocket {

int m_server;
int m_client;

struct sockaddr_in *m_server_addr;
struct sockaddr_in *m_client_addr;

protected:
	std::string m_host;
	std::string m_origin;
	std::string m_protocol;
	std::string m_key;
	std::string m_accept;

public:
	WebSocket(int server, struct sockaddr_in *server_addr, int client, struct sockaddr_in *client_addr);
	virtual ~WebSocket();

	std::string AcceptKey();
	std::string AcceptKey(std::string original);

	virtual void Handshake();

	virtual void Listen();
	virtual void Process(std::string message);
	
};

#endif