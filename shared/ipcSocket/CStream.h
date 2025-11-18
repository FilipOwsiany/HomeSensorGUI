#ifndef C_STREAM_H
#define C_STREAM_H

#include <iostream>
#include <string>
#include <cstring>

class CStream
{
private:

    uint8_t* mBuffer = nullptr;
    uint16_t mCapacity = 0;   // ile fizycznie mieści bufor
    uint16_t mSize = 0;       // ile bajtów zostało zapisane
    uint16_t mPosition = 0;   // gdzie jesteśmy
    bool mIsError = false;

    void printfBuffer()
    {
        #ifdef CSTREAMDEBUG
            for (uint16_t i = 0; i < mSize; i++)
            {
                std::cout << std::hex << static_cast<int>(mBuffer[i]) << " "; 
            }
            std::cout << std::endl;
        #endif
    }
public:

    enum class EDirection
    {
        IN = 0,
        OUT,
        IN_OUT,
    };

    CStream(uint8_t* aBuffer, uint16_t aCapacity, EDirection aDirection);
    ~CStream();

    template<typename T>
    CStream& operator<<(T aValue);
    template<typename T>
    CStream& operator>>(T& aValue);

    CStream& operator<<(const std::string& aValue);
    CStream& operator>>(std::string& aValue);

    bool isError() const { return mIsError; }
    uint16_t getSize() const { return mSize; }
    uint16_t getCapacity() const { return mCapacity; }
    uint16_t getPosition() const { return mPosition; }

    void rewind() { mPosition = 0; }

    uint16_t leftToWrite() const { return mCapacity - mSize; }
    uint16_t leftToRead()  const { return mSize - mPosition; }

    uint8_t* getBuffer() { return mBuffer; }
};

template<typename T>
CStream& CStream::operator<<(T aValue)
{
    if (mIsError || leftToWrite() < sizeof(T))
    {
        return *this;
    }

    std::memcpy(&mBuffer[mSize], &aValue, sizeof(T));
    mSize += sizeof(T);

    printfBuffer();

    return *this;
}

template<typename T>
CStream& CStream::operator>>(T& aValue)
{
    printf("1 CStream::operator>>\n");

    if (mIsError || (leftToRead() < sizeof(T)))
    {
        return *this;
    }

    printf("2 CStream::operator>>\n");

    std::memcpy(&aValue, &mBuffer[mPosition], sizeof(T));
    mPosition += sizeof(T);

    return *this;
}


#endif // C_STREAM_H

