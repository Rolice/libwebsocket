#include "Callback.hpp"

namespace ws
{

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

void CallbackManager::Trigger(CallbackType type)
{
	for(CallbackCollectionIterator it = m_collection.begin(); it != m_collection.end(); ++it)
	{
		if(type != it->second.type)
			continue;

		switch(type)
		{
			case CT_CONNECTED:
				(ConnectedCallback) it->second.callback;
				break;
			case CT_DISCONNECTED:

				break;
			case CT_FRAME:

				break;
			case CT_MESSAGE:

				break;
			default:
				Debug::Warn("Unknown Callback Invoked");
				break;
		}
	}
}

}