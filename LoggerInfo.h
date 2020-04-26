#ifndef LOGGERINFO_H_
#define LOGGERINFO_H_

#include "Logger.h"

class LoggerInfo: public Logger{
    public:
        LoggerInfo(const char* filename);
        void info(const char* update);
};

#endif
