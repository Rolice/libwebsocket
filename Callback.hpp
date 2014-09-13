#ifndef __CALLBACK_HPP__
#define __CALLBACK_HPP__

#include "Frame.hpp"

#include <map>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>

namespace ws
{

enum CallbackType
{
	CT_CONNECTED,
	CT_DISCONNECTED,
	CT_FRAME,
	CT_MESSAGE
};

typedef void (*ConnectedCallback)(struct ClientInfo &info, std::string handshake_key);
typedef void (*DisconnectedCallback)(struct ClientInfo &info);

typedef void (*FrameCallback)(struct ClientInfo &info, ws::Frame &frame);
typedef void (*MessageCallback)(struct ClientInfo &info, std::string &message);

typedef unsigned int CallbackHandle;

struct CallbackInfo
{
	CallbackType type;
	CallbackHandle handle;
	void *callback;

	CallbackInfo();
	CallbackInfo(CallbackType type, CallbackHandle handle, void (*callback)())
	{
		type = type;
		handle = handle;
		callback = callback;
	}
};

typedef std::pair<CallbackHandle, CallbackInfo> ConnectedCallbackItem;
typedef std::pair<CallbackHandle, CallbackInfo> DisconnectedCallbackItem;
typedef std::pair<CallbackHandle, CallbackInfo> FrameCallbackItem;
typedef std::pair<CallbackHandle, CallbackInfo> MessageCallbackItem;

typedef std::map<CallbackHandle, CallbackInfo> ConnectedCallbackCollection;
typedef std::map<CallbackHandle, CallbackInfo> DisconnectedCallbackCollection;
typedef std::map<CallbackHandle, CallbackInfo> FrameCallbackCollection;
typedef std::map<CallbackHandle, CallbackInfo> MessageCallbackCollection;

struct ClientInfo
{
	int descriptor;
	struct sockaddr_in address;
};

class CallbackManager
{
private:
	ConnectedCallbackCollection m_connected_collection;
	DisconnectedCallbackCollection m_disconnected_collection;
	FrameCallbackCollection m_frame_collection;
	MessageCallbackCollection m_message_collection;

	CallbackHandle Generate();
	CallbackHandle Generate(int priority);

public:
	static const int DefaultPriority = 50;

	CallbackHandle RegisterCallback(ConnectedCallback callback);
	CallbackHandle RegisterCallback(ConnectedCallback callback, int priority);
	CallbackHandle RegisterCallback(DisconnectedCallback callback);
	CallbackHandle RegisterCallback(DisconnectedCallback callback, int priority);
	CallbackHandle RegisterCallback(FrameCallback callback);
	CallbackHandle RegisterCallback(FrameCallback callback, int priority);
	CallbackHandle RegisterCallback(MessageCallback callback);
	CallbackHandle RegisterCallback(MessageCallback callback, int priority);

	void UnregisterCallback(CallbackHandle handle);
};

}

#endif