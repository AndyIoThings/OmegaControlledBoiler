#ifndef SERVER_HPP
#define SERVER_HPP
#include "../Configuration/server_config.h"

typedef bool (*StartFunc)();
struct Server
{
   StartFunc start;
};

struct Server* getServer(int port);
void setCertPath(char *certPath);
void setKeyPath(char *keyPath);
void setPathForCA(char *pathForCA);

;

#endif // SERVER_HPP