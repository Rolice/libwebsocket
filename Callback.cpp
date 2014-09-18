#include "Callback.hpp"

namespace ws
{

CallbackCollection CallbackManager::m_collection;

CallbackHandle CallbackManager::Generate()
{
	CallbackHandle result = 0;
	CallbackHandle *p = &result;
	
	Util::RandomRaw((void *) p, sizeof(result));

	return result;
}

CallbackHandle CallbackManager::RegisterCallback(ConnectedCallback callback)
{

	return RegisterCallback(callback, CallbackManager::DefaultPriority);
}

CallbackHandle CallbackManager::RegisterCallback(ConnectedCallback callback, int priority)
{
	CallbackHandle handle = Generate();
	CallbackInfo info;

	info.type = CT_CONNECTED;
	info.handle = handle;
	info.callback = (void *) callback;

	m_collection.insert(CallbackItem(priority, info));

	return handle;
}

CallbackHandle CallbackManager::RegisterCallback(DisconnectedCallback callback)
{

	return RegisterCallback(callback, CallbackManager::DefaultPriority);
}

CallbackHandle CallbackManager::RegisterCallback(DisconnectedCallback callback, int priority)
{
	CallbackHandle handle = Generate();
	CallbackInfo info;

	info.type = CT_DISCONNECTED;
	info.handle = handle;
	info.callback = (void *) callback;

	m_collection.insert(CallbackItem(priority, info));

	return handle;
}

CallbackHandle CallbackManager::RegisterCallback(FrameCallback callback)
{

	return RegisterCallback(callback, CallbackManager::DefaultPriority);
}

CallbackHandle CallbackManager::RegisterCallback(FrameCallback callback, int priority)
{
	CallbackHandle handle = Generate();
	CallbackInfo info;

	info.type = CT_FRAME;
	info.handle = handle;
	info.callback = (void *) callback;

	m_collection.insert(CallbackItem(priority, info));

	return handle;
}

CallbackHandle CallbackManager::RegisterCallback(MessageCallback callback)
{

	return RegisterCallback(callback, CallbackManager::DefaultPriority);
}

CallbackHandle CallbackManager::RegisterCallback(MessageCallback callback, int priority)
{
	CallbackHandle handle = Generate();
	CallbackInfo info;

	info.type = CT_MESSAGE;
	info.handle = handle;
	info.callback = (void *) callback;

	m_collection.insert(CallbackItem(priority, info));

	return handle;
}

void CallbackManager::UnregisterCallback(CallbackHandle handle)
{

}

void CallbackManager::Trigger(CallbackType type, ClientInfo info, ...)
{
	va_list args;
	va_start(args, info);

	for(CallbackCollectionIterator it = m_collection.begin(); it != m_collection.end(); ++it)
	{
		if(type != it->second.type)
			continue;

		switch(type)
		{
			case CT_CONNECTED:
			{
				ConnectedCallback callback = (ConnectedCallback) it->second.callback;
				callback(info, va_arg(args, char *));

				break;
			}
			case CT_DISCONNECTED:
			{
				DisconnectedCallback callback = (DisconnectedCallback) it->second.callback;
				callback(info);

				break;
			}
			case CT_FRAME:
			{
				Frame *frame = va_arg(args, Frame *);

				FrameCallback callback = (FrameCallback) it->second.callback;
				callback(info, frame);

				break;
			}
			case CT_MESSAGE:
			{
				char *message = va_arg(args, char *);
				size_t length = va_arg(args, size_t);

				MessageCallback callback = (MessageCallback) it->second.callback;
				callback(info, message, length);

				break;
			}
			default:
			{
				Debug::Warn("Unknown Callback Invoked");
				break;
			}
		}
	}
}

}