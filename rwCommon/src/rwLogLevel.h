/**
* \author       Robert Widziszewski https://github.com/softhairspring
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef IncLog
#define IncLog
/*
* uzywane przez FileParser (?)
*/
enum LogLevel { LOG_QUIET, LOG_ERROR, LOG_INFO, LOG_DEBUG };

extern LogLevel logLevel;

#define logError(A) ((logLevel >= LOG_ERROR)?((A),0):(0))
#define logInfo(A) ((logLevel >= LOG_INFO)?((A),0):(0))
#define logDebug(A) ((logLevel >= LOG_DEBUG)?((A),0):(0))

void debugBreak();

#endif

