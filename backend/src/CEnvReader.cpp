#include "CEnvReader.h"

#include <iostream>

CEnvReader::CEnvReader(std::string aFileName, std::string aFilePath) :
 mFileName(aFileName), mFilePath(aFilePath), mFile(nullptr)
{

}

CEnvReader::~CEnvReader()
{
    if (mFile != nullptr)
    {
        delete mFile;
    }
}

std::string CEnvReader::readLine(int aLine, size_t aMaxLen)
{
    if (!isGoodFile())
    {
        return "";
    }

    mFile->clear();
    mFile->seekg(0);

    int currentLine = 1;
    std::string line;
    while (std::getline(*mFile, line)) 
    {
        if (currentLine == aLine) 
        {
            if (line.size() > aMaxLen) 
            {
                return "";
            }
            return line;
        }
        currentLine++;
    }
    return "";
}

void CEnvReader::openFile()
{
    std::string fileToOpen = mFilePath + mFileName;
    if(mFile == nullptr)
    {
        mFile = new std::ifstream(fileToOpen);
    }
}

bool CEnvReader::readFile()
{
    
    mEnvReadedData.mLoginUrl = readLine(1, 300);
    if (mEnvReadedData.mLoginUrl == "")
    {
        return false;
    }
    mEnvReadedData.mDataUrl = readLine(2, 300);
    if (mEnvReadedData.mDataUrl == "")
    {
        return false;
    }
    mEnvReadedData.mEmail = readLine(3, 300);
    if (mEnvReadedData.mEmail == "")
    {
        return false;
    }
    mEnvReadedData.mPassword = readLine(4, 300);
    if (mEnvReadedData.mPassword == "")
    {
        return false;
    }

    return true;
}

const CEnvData& CEnvReader::getEnvReadedData()
{
    return mEnvReadedData;
}