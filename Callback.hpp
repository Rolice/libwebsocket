#ifndef __CALLBACK_HPP__
#define __CALLBACK_HPP__

#include "Frame.hpp"
#include "Debug.hpp"

#include <stdarg.h>

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

typedef void (*ConnectedCallback)(struct ClientInfo &info, char *handshake_key);
typedef void (*DisconnectedCallback)(struct ClientInfo &info);
typedef void (*FrameCallback)(struct ClientInfo &info, ws::Frame *frame);
typedef void (*MessageCallback)(struct ClientInfo &info, char *message, size_t length);

typedef unsigned int CallbackHandle;

struct CallbackInfo
{
	CallbackType type;
	CallbackHandle handle;
	void *callback;

	CallbackInfo()
	{
	}

	CallbackInfo(CallbackType type, CallbackHandle handle, void (*callback)())
	{
		type = type;
		handle = handle;
		callback = callback;
	}
};

struct ClientInfo
{
	int descriptor;
	struct sockaddr_in address;
};

typedef std::multimap<int, CallbackInfo> CallbackCollection;
typedef std::multimap<int, CallbackInfo>::iterator CallbackCollectionIterator;
typedef std::pair<int, CallbackInfo> CallbackItem;

class CallbackManager
{
private:
	static CallbackCollection m_collection;
	
	static CallbackHandle Generate();

public:
	static const int DefaultPriority = 50;

	static CallbackHandle RegisterCallback(ConnectedCallback callback);
	static CallbackHandle RegisterCallback(ConnectedCallback callback, int priority);
	static CallbackHandle RegisterCallback(DisconnectedCallback callback);
	static CallbackHandle RegisterCallback(DisconnectedCallback callback, int priority);
	static CallbackHandle RegisterCallback(FrameCallback callback);
	static CallbackHandle RegisterCallback(FrameCallback callback, int priority);
	static CallbackHandle RegisterCallback(MessageCallback callback);
	static CallbackHandle RegisterCallback(MessageCallback callback, int priority);

	static void UnregisterCallback(CallbackHandle handle);

	static void Trigger(CallbackType type, ClientInfo info, ...);

};

}

#endif