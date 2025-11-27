#ifndef C_ENV_READER_H
#define C_ENV_READER_H

#include <string>
#include <cstdio>
#include <fstream>

#include "CEnvData.h"

class CEnvReader
{
private:
    std::string     mFileName;
    std::string     mFilePath;
    std::ifstream*  mFile;
    CEnvData        mEnvReadedData;

    std::string readLine(int aLine, size_t aMaxLen);

public:
    CEnvReader(std::string aFileName, std::string aFilePath);
    ~CEnvReader();
    void openFile();
    bool readFile();
    bool isGoodFile() { return mFile != nullptr && mFile->is_open() && mFile->good(); }
    const CEnvData& getEnvReadedData();
};

#endif