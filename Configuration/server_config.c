#include "server_config.h"

static struct ServerConfig serverConfig = {.IPAddress = DEFAULT_SERVER_IP_ADDRESS,
.port = DEFAULT_LISTENING_PORT,
.rootDir = DEFAULT_ROOT_DIR};

const struct ServerConfig GetServerConfig(void)
{
	struct ServerConfig servConf = serverConfig;
	return servConf;
}

void SetServerConfigIPAddress(const char *ipadd)
{
	if (ipadd)
		strcpy(serverConfig.IPAddress, ipadd);
}

void SetServerConfigPort(const unsigned int port) { serverConfig.port = port; }

void SetServerConfigRootDir(const char *rootDir)
{
	if (rootDir)
		strcpy(serverConfig.rootDir, rootDir);
}

int SaveConfigToFile(void)
{
	// DEFAULT_CONFIG_FILE
	return 0;
}

int LoadConfigFromFile(void) { return 0; }
