#include "CStream.h"

CStream::CStream(uint8_t* aBuffer, uint16_t aCapacity, EDirection aDirection)
  : mBuffer(aBuffer),
    mCapacity(aCapacity),
    mSize(0),
    mPosition(0)
{
    if (!mBuffer || mCapacity == 0)
    {
        mIsError = true;
    }

    if (aDirection == EDirection::OUT)
    {
        mSize = mCapacity;
    }
}

CStream::~CStream()
{
}

CStream& CStream::operator<<(const std::string& aValue)
{
    uint16_t strSize = static_cast<uint16_t>(aValue.size());
    (*this) << strSize;

    if (mIsError || (leftToWrite() < strSize))
    {
        return *this;
    }

    std::memcpy(&mBuffer[mSize], aValue.c_str(), strSize);
    mSize += strSize;

    printfBuffer();

    return *this;
}

CStream& CStream::operator>>(std::string& aValue)
{
    uint16_t strLen = 0;
    (*this) >> strLen;

    if (mIsError || (leftToRead() < strLen))
    {
        mIsError = true;
        return *this;
    }

    aValue.assign(reinterpret_cast<char*>(&mBuffer[mPosition]), strLen);
    mPosition += strLen;

    return *this;
}