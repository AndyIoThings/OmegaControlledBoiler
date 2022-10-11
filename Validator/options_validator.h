#ifndef OPTIONS_VALIDATOR_H
#define OPTIONS_VALIDATOR_H
#include "../pre_header.h"

bool validateIP4Address(const char *data, const size_t dataLength);
bool validateListenPort(const char *data, const size_t dataLength);
bool validateRootFolderPath(const char *data, const size_t dataLength);

#endif // OPTIONS_VALIDATOR_H
