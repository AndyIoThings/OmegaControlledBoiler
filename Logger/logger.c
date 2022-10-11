#include "logger.h"

int terminalLog(const char *data)
{
	const time_t currentTime = time(NULL);
	struct tm result;
	struct tm *currentTimeStruct = localtime_r(&currentTime, &result);
	char buf[256];
	char *textTime = asctime_r(currentTimeStruct, buf);
	char *ret = strchr(textTime, '\n');

	if (ret)
		*ret = '\0';
	return printf("[%s] - %s", textTime, data);
}

int logData(enum ELogType type, const char *fmt, ...)
{
	if (fmt) {
		va_list args1;

		va_start(args1, fmt);
		va_list args2;

		va_copy(args2, args1);
		char buf[1024];

		va_end(args1);
		vsnprintf(buf, sizeof(buf), fmt, args2);
		va_end(args2);
		switch (type) {
		case SERVER_LOG:
			return terminalLog(buf); // change
		case DEBUG_LOG:
			return terminalLog(buf);
		}
	}
}
