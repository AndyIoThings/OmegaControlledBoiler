#ifndef SERVER_HPP
#define SERVER_HPP
#include "../Configuration/server_config.h"

typedef bool (*StartFunc)();
struct Server
{
   StartFunc start;
};

struct Server* getServer(int port);
void setCertPath(const char *certPath);
void setKeyPath(const char *keyPath);
void setPathForCA(const char *pathForCA);

;

#endif // SERVER_HPP