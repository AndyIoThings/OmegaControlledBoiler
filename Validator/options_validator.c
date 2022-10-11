#include "options_validator.h"
#include "../Logger/logger.h"

bool validateIP4Address(const char *pdata, const size_t dataLength)
{
	if (pdata) {
		if (strlen(pdata) == dataLength) {
			const char *valid = "0123456789.";
			size_t valid_len = strspn(pdata, valid);

			if (valid_len != dataLength) {
				logData(SERVER_LOG,
								"'%s' contains invalid chars starting at position %zu\n", pdata,
								valid_len);
				return false;
			}
			char buf[IP4ADDRESS_TOKEN_MAX_CHAR_NUMBER];

			memset(buf, 0, IP4ADDRESS_TOKEN_MAX_CHAR_NUMBER);
			memcpy(buf, pdata, dataLength);
			const char *delim = ".";
			const size_t tokenCount = IP4ADDRESS_TOKEN_COUNT;
			size_t i = 0;

			const size_t cforthToken = 3;

			logData(DEBUG_LOG, "Parsing the input string '%s'\n", pdata);
			char *token = NULL;

			token = strtok(buf, delim);
			while (token) {
				int iToken = atoi(token);

				if (IP4ADDRESS_TOKEN_MAX_NUMBER < iToken)
					return false;
				if ((IP4ADDRESS_TOKEN_MIN_NUMBER > iToken) && (cforthToken == i))
					return false;
				token = strtok(NULL, delim);
				++i;
			}
			if (tokenCount == i)
				return true;
			else
				return false;
		}
	} else {
		return false;
	}
}

bool validateListenPort(const char *pdata, const size_t dataLength)
{
	if (pdata) {
		if (strlen(pdata) == dataLength) {
			int itestPort = atoi(pdata);

			if ((PORT_NUMBER_MIN > itestPort) || (itestPort > PORT_NUMBER_MAX))
				return false;
		}
		return true;
	} else
		return false;
}

bool validateRootFolderPath(const char *pdata, const size_t dataLength)
{
	if (pdata) {
		if (strlen(pdata) == dataLength) {
			const char *invalid = "\\'";
			size_t valid_len = strcspn(pdata, invalid);

			if (valid_len != dataLength) {
				logData(SERVER_LOG,
								"'%s' contains invalid chars starting at position %zu\n", pdata,
								valid_len);
				return false;
			}
			if (dataLength > ROOT_DIR_MAX_PATH_LENGTH)
				return false;
			char buf[ROOT_DIR_MAX_PATH_LENGTH];
			memset(buf, 0, ROOT_DIR_MAX_PATH_LENGTH);
			memcpy(buf, pdata, dataLength);
			const char *delim = "/";
			logData(DEBUG_LOG, "Parsing the input string '%s'", pdata);
			char *token = NULL;
			token = strtok(buf, delim);
			while (token) {
				int iToken = atoi(token);
				if (DIR_MAX_NAME_LENGTH < iToken)
					return false;
				token = strtok(NULL, delim);
			}
			return true;
		}
	} else
		return false;
}
