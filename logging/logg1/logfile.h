#ifndef __LOGFILE__
#define __LOGFILE__

typedef enum {
  DEBUG1,
  DEBUG2,
  DEBUG3,
  WARNING,
  ERROR,
} DEBUG_LEVEL;

#define LOG_PRINT(_level, txt...) \
        if (debugFlagOn == 1 && _level < debugLevel) { \
          printf("%s - [%s %d]: ", __func__, __FILE__, __LINE__); \
          internalLogPrintf(txt); \
        } \
        else if (debugFlagOn == 1 && _level == WARNING) { \
          printf("%s - [%s %d]: ", __func__, __FILE__, __LINE__); \
          internalLogPrintf(txt); \
        } \
        else if (_level == ERROR) { \
          printf("%s - [%s %d]: ", __func__, __FILE__, __LINE__); \
          internalLogPrintf(txt); \
        }

#endif // __LOGFILE__
