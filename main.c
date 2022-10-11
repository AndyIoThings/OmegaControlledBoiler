#include "pre_header.h"

#include "Configuration/server_config.h"
#include "Logger/logger.h"
#include "OptionsParser/OptionsParser.h"
#include "Server/server.h"

int main(int argc, char *argv[]) {

  logData(SERVER_LOG, "Http Server. Version 1.0.%d\n", 0);
  ParseOptions(argc, argv);
  char *pathToCert ="./certificates/selfsigned.crt";
  char *pathToKey = "./certificates/selfsigned.key";

  setCertPath(pathToCert);
  setKeyPath(pathToKey);
  struct ServerConfig servConf = GetServerConfig();
  struct Server *serv = getServer(servConf.port);
  serv->start();

  logData(SERVER_LOG, "Http Server address: %s\n", servConf.IPAddress);
  logData(SERVER_LOG, "Http Server port: %d\n", servConf.port);
  logData(SERVER_LOG, "Http Server root directory: %s\n", servConf.rootDir);
  return EXIT_SUCCESS;
}
