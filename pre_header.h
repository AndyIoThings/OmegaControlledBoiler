#ifndef PRE_HEADER
#define PRE_HEADER

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define DEFAULT_ROOT_DIR "./httpserver_data/www"
#define DEFAULT_CONFIG_FILE "./httpserver_data/server.conf"
#define DEFAULT_SERVER_IP_ADDRESS "127.0.0.1"
#define DEFAULT_LISTENING_PORT 80
#define LOGBUFFER_SIZE 1024

#define IP4ADDRESS_TOKEN_COUNT 4
#define IP4ADDRESS_TOKEN_MAX_NUMBER 254
#define IP4ADDRESS_TOKEN_MIN_NUMBER 1
#define IP4ADDRESS_TOKEN_MAX_CHAR_NUMBER 18

#define PORT_NUMBER_MIN 80
#define PORT_NUMBER_MAX 50000

#define ROOT_DIR_MAX_PATH_LENGTH 4095
#define DIR_MAX_NAME_LENGTH 255

#if !defined(SERVER_VERSION)
#define SERVER_VERSION 0
#endif

#endif // PRE_HEADER
