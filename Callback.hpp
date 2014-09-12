#ifndef __CALLBACK_HPP__
#define __CALLBACK_HPP__

#include "Frame.hpp"

#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>

namespace ws
{

typedef void (*ConnectedCallback)(struct ClientInfo &info, std::string handshake_key);
typedef void (*DisconnectedCallback)(struct ClientInfo &info);

typedef void (*FrameCallback)(struct ClientInfo &info, ws::Frame &frame);
typedef void (*MessageCallback)(struct ClientInfo &info, std::string &message);

struct ClientInfo
{
	int descriptor;
	struct sockaddr_in address;
};

}

#endif