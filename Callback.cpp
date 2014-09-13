#include "Callback.hpp"

namespace ws
{

CallbackHandle CallbackManager::Generate()
{
	return Generate(CallbackManager::DefaultPriority);
}

CallbackHandle CallbackManager::Generate(int priority)
{
	CallbackHandle result = 0;
	CallbackHandle *p = &result;
	
	Util::RandomRaw((void *) p, sizeof(result / 2));

	p += sizeof(result / 2);

	for(size_t i = 0; i < sizeof(result / 2); i++)
		*p++ = priority;

	return result;
}

CallbackHandle CallbackManager::RegisterCallback(ConnectedCallback callback)
{
	return RegisterCallback(callback, CallbackManager::DefaultPriority);
}

CallbackHandle CallbackManager::RegisterCallback(ConnectedCallback callback, int priority)
{
	CallbackHandle handle = Generate(priority);
	CallbackInfo info;

	info.type = CT_CONNECTED;
	info.handle = handle;
	info.callback = (void *) callback;

	m_frame_collection.insert(FrameCallbackItem(priority, info));

	return handle;
}

CallbackHandle CallbackManager::RegisterCallback(DisconnectedCallback callback)
{
	return RegisterCallback(callback, CallbackManager::DefaultPriority);
}

CallbackHandle CallbackManager::RegisterCallback(DisconnectedCallback callback, int priority)
{
	CallbackHandle handle = Generate(priority);
	CallbackInfo info;

	info.type = CT_DISCONNECTED;
	info.handle = handle;
	info.callback = (void *) callback;

	m_frame_collection.insert(FrameCallbackItem(priority, info));

	return handle;
}

CallbackHandle CallbackManager::RegisterCallback(FrameCallback callback)
{
	return RegisterCallback(callback, CallbackManager::DefaultPriority);
}

CallbackHandle CallbackManager::RegisterCallback(FrameCallback callback, int priority)
{
	CallbackHandle handle = Generate(priority);
	CallbackInfo info;

	info.type = CT_FRAME;
	info.handle = handle;
	info.callback = (void *) callback;

	m_frame_collection.insert(FrameCallbackItem(priority, info));

	return handle;
}

CallbackHandle CallbackManager::RegisterCallback(MessageCallback callback)
{
	return RegisterCallback(callback, CallbackManager::DefaultPriority);
}

CallbackHandle CallbackManager::RegisterCallback(MessageCallback callback, int priority)
{
	CallbackHandle handle = Generate(priority);
	CallbackInfo info;

	info.type = CT_MESSAGE;
	info.handle = handle;
	info.callback = (void *) callback;

	m_frame_collection.insert(FrameCallbackItem(priority, info));

	return handle;
}

void CallbackManager::UnregisterCallback(CallbackHandle handle)
{

}

}