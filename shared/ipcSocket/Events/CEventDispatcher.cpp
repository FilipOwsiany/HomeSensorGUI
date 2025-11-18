#include "CEventDispatcher.h"

void CEventDispatcher::registerCallback(IEventCallback* aCallback, EEventType aEventType)
{
    CEventDispatcherMetadata* metadata = new CEventDispatcherMetadata();
    metadata->callback = aCallback;
    metadata->eventType = aEventType;
    mCallbacksMetadata.push_back(metadata);
}

void CEventDispatcher::unregisterCallback(EEventType aEventType)
{
    mCallbacksMetadata.erase(
        std::remove_if(
            mCallbacksMetadata.begin(),
            mCallbacksMetadata.end(),
            [aEventType](CEventDispatcherMetadata* metadata)
            {
                if (metadata->eventType == aEventType)
                {
                    delete metadata;
                    return true;
                }
                return false;
            }
        ),
        mCallbacksMetadata.end()
    );
}

void CEventDispatcher::dispatchEvent(CEventBase* event)
{
    for (auto& callbackMetadata : mCallbacksMetadata)
    {
        callbackMetadata->callback->onEvent(event);
    }
}