#ifndef LOGGERINFO_H_
#define LOGGERINFO_H_

#include "Logger.h"

class LoggerInfo: public Logger{
    public:
        LoggerInfo(const char* filename);
        void debug(const char* update);
        void info(const char* update);
        void error(const char* update);
};

#endif
