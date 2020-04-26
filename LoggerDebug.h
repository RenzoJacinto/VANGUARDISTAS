#ifndef LOGGERDEBUG_H_
#define LOGGERDEBUG_H_

#include "Logger.h"

class LoggerDebug: public Logger{
    public:
        LoggerDebug(const char* filename);
};

#endif
