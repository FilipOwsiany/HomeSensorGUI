#ifndef C_EVENT_DISPATCHER_H
#define C_EVENT_DISPATCHER_H

#include "IEventCallback.h"
#include "CEventBase.h"

#include "vector"
#include "algorithm"

class CEventDispatcher
{
private:

    struct CEventDispatcherMetadata
    {
        IEventCallback* callback;
        EEventType eventType;
    };
    
    std::vector<CEventDispatcherMetadata*> mCallbacksMetadata;
protected:

    CEventDispatcher() = default;
    ~CEventDispatcher() = default;
    CEventDispatcher(const CEventDispatcher&) = delete;
    CEventDispatcher& operator=(const CEventDispatcher&) = delete;

public:
    static CEventDispatcher& getInstance()
    {
        static CEventDispatcher instance;
        return instance;
    }

    void registerCallback(IEventCallback* aCallback, EEventType aEventType);
    void unregisterCallback(EEventType aEventType);
    void dispatchEvent(CEventBase* event);
};


#endif // C_EVENT_DISPATCHER_H