#ifndef LOGGERDEBUG_H_
#define LOGGERDEBUG_H_

#include "Logger.h"

class LoggerDebug: public Logger{
    public:
        LoggerDebug(const char* filename);
        void debug(const char* update);
        void info(const char* update);
        void error(const char* update);
};

#endif
