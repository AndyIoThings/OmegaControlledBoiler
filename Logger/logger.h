#ifndef LOGGER_H
#define LOGGER_H
#include "../pre_header.h"

enum ELogType { SERVER_LOG, DEBUG_LOG };

int logData(enum ELogType type, const char *fmt, ...);

#endif // LOGGER_H
