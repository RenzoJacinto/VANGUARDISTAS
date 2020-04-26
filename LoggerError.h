#ifndef LOGGERERROR_H_
#define LOGGERERROR_H_

#include "Logger.h"

class LoggerError: public Logger{
    public:
        LoggerError(const char* filename);
        void error(const char* update);
};

#endif
