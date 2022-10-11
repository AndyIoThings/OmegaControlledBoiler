#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include "../pre_header.h"

struct ServerConfig {
	char IPAddress[IP4ADDRESS_TOKEN_MAX_CHAR_NUMBER];
	unsigned int port;
	char rootDir[ROOT_DIR_MAX_PATH_LENGTH];
};

const struct ServerConfig GetServerConfig(void);

void SetServerConfigIPAddress(const char *ipadd);
void SetServerConfigPort(const unsigned int port);
void SetServerConfigRootDir(const char *rootDir);

int SaveConfigToFile(void);
int LoadConfigFromFile(void);

#endif // SERVER_CONFIG_H
