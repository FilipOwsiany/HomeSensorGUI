#ifndef C_EVENTS_H
#define C_EVENTS_H

#include "CEventBase.h"

class CEventTemperature : public CEventBase
{
private:
    uint32_t mTemperature = 0;
public:
    CEventTemperature();
    ~CEventTemperature();
    uint32_t getTemperature() const { return mTemperature; }
    void setTemperature(uint32_t aTemperature) { mTemperature = aTemperature; }

    bool serialize(CStream& aStream) override;
    bool unserialize(CStream& aStream) override;
};

#endif // C_EVENTS_H