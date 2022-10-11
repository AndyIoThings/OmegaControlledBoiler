#include "../Server/server.h"
#include "../RequestHandler/requestParser.h"

#include "../Logger/logger.h"
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <openssl/err.h>
#include <openssl/ssl.h>

//static bool serverSSLInit();
static bool serverSocketInit();
static void serverClientProcessing(int, char *);
static void serverClientProcessing_(int, char *);
static int serverSocket = -1;
static int serverPort = 443;

enum eDataSizeConstants {
  htmlFileSize = 1024 * 1024 * 1,
  iconFileSize = 1024 * 8 * 4,
  pathBufSize = 1024,
  responseSize = 1024 * 1024 * 1,
  uriSize = 1024,
  dateTimeBuf = 1024,
  chunkThreshold = 1024 * 512,
  CMD_SIZE = 32,
  DELIM_SIZE = 8,
  VALUE_SIZE = 128
};

static char serverSSLCertificatePath[pathBufSize];
static char serverSSLkeyPath[pathBufSize];
static char caPath[pathBufSize];
static char htmlFileData[htmlFileSize];
static size_t actualIconFileSize = 0;
static char startDateTime[dateTimeBuf];
static size_t requestCount = 0;
static SSL_METHOD *pSSLMethod = {NULL};
static SSL_CTX *pSSLContext = {NULL};
static char iconFavBuffer[iconFileSize];

static void *inAddr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET)
    return &(((struct sockaddr_in *)sa)->sin_addr);

  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

static void ipAddr(const struct sockaddr_in clientAddr, char *addrBuf) {
  char ip[INET6_ADDRSTRLEN] = {0};
  inet_ntop(clientAddr.sin_family, inAddr((struct sockaddr *)&clientAddr), ip,
            sizeof ip);
  memcpy(addrBuf, ip, strlen(ip));
}
static void splitAndSend(char* data, size_t chunkSize){

size_t dataLength = strlen(data);
size_t chunkAmount = (dataLength%(chunkSize - 4) == 0)? dataLength/(chunkSize - 4) :
 (dataLength/(chunkSize - 4)) + 1;
char response[chunkSize];
char chunkBuf[chunkSize];
memset(response, 0, chunkSize);
size_t i = 0;

while (i != chunkAmount){
    
  char* chunkCursor = data + i*chunkSize;
  strncpy(response, chunkCursor, chunkSize);
  
  i++;
}

}
static void dateTime(char *curDateTime) {
  char buffer[100] = {0};
  time_t curr_time;
  struct tm curr_tm = {};
  time(&curr_time);
  localtime_r(&curr_time, &curr_tm);
  strftime(buffer, 100, "%T %d.%m.%Y", &curr_tm);
  memcpy(curDateTime, buffer, 100);
}
bool startServer();
static struct Server httpServer = {.start = startServer};

struct Server *getServer(int port) {
  serverPort = port;
  return &httpServer;
}

bool startServer() {
  const size_t bufSize = 1024;
  char currentBinPathBuf[bufSize];

  getcwd(currentBinPathBuf, bufSize);

  char *htmlFileName = "index.html";
  char *htmlFileDirName = "html_data";
  char pathToHtmlFile[bufSize];
  memset(pathToHtmlFile, 0, bufSize);
  sprintf(pathToHtmlFile, "%s/%s/%s", currentBinPathBuf, htmlFileDirName,
          htmlFileName);
  int htmlFileDisc = -1;

  if ((htmlFileDisc = open(pathToHtmlFile, O_RDONLY)) == -1) {
    size_t buflen = 1024;
    char buf[buflen];
    strerror_r(errno, buf, buflen);
    logData(SERVER_LOG, "Cannot open %s, error: %s\n", pathToHtmlFile, buf);
    return false;
  }

  int ret_in = read(htmlFileDisc, htmlFileData, htmlFileSize);
  close(htmlFileDisc);

  int iconFileDisc = -1;
  char *iconFileName = "favicon.png";
  char *iconFileDirName = "icons";
  char pathToIconFile[bufSize];
  memset(pathToIconFile, 0, bufSize);

  sprintf(pathToIconFile, "%s/%s/%s", currentBinPathBuf, iconFileDirName,
          iconFileName);

  if ((iconFileDisc = open(pathToIconFile, O_RDONLY)) == -1) {
    logData(SERVER_LOG, "Cannot open %s\n", pathToIconFile);
    return false;
  }
  int ret = -1;
  struct stat buf;
  fstat(iconFileDisc, &buf);
  actualIconFileSize = buf.st_size;

  logData(SERVER_LOG, "Favicon size %d!\n", actualIconFileSize);
  if ((ret = read(iconFileDisc, iconFavBuffer, actualIconFileSize)) == -1) {
    logData(SERVER_LOG, "Favicon read error!\n");
  }
  close(iconFileDisc);

  // if (!serverSSLInit()) {
  //   logData(SERVER_LOG, "Ssl init error\n");
  //   return false;
  // }

  if (!serverSocketInit()) {
    logData(SERVER_LOG, "Socket init error\n");
    return false;
  }

  dateTime(startDateTime);
  logData(SERVER_LOG, "Server was started, port:%d\n", serverPort);

  while (1) {
    int clientSocket;
    struct sockaddr_in sa_cli;
    socklen_t client_len = sizeof(sa_cli);

    clientSocket =
        accept(serverSocket, (struct sockaddr *)&sa_cli, &client_len);
    logData(SERVER_LOG, "clientSocket %d\n", clientSocket);
    if (clientSocket == -1) {
      logData(SERVER_LOG, "accept error:\n");
      continue;
    }
    char addrBuf[bufSize];
    memset(addrBuf, 0, bufSize);
    ipAddr(sa_cli, addrBuf);
    serverClientProcessing_(clientSocket, addrBuf);
  }
  return true;
}
//------------------------------------------------------------------------------
bool serverSocketInit() {
  serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  if (serverSocket == -1) {
    logData(SERVER_LOG, "Can't create socket\n");
    return false;
  }

  struct sockaddr_in sa_serv;
  memset(&sa_serv, 0, sizeof(sa_serv));
  sa_serv.sin_family = AF_INET;
  sa_serv.sin_addr.s_addr = INADDR_ANY;
  sa_serv.sin_port = htons(serverPort);

  if (bind(serverSocket, (struct sockaddr *)&sa_serv, sizeof(sa_serv)) != 0) {
    size_t buflen = 1024;
    char buf[buflen];
    strerror_r(errno, buf, buflen);
    logData(SERVER_LOG, "Can't bind port %d, error:%s\n", serverPort, buf);
    return false;
  }

  if (listen(serverSocket, 1000) != 0) {
    logData(SERVER_LOG, "Can't configure listening port\n");
    return false;
  }
  return true;
}
//------------------------------------------------------------------------------
void serverClientProcessing(int SocketValue, char *IpAddress) {
  SSL *ssl;
  ssl = SSL_new(pSSLContext); // get new SSL state with context
  logData(SERVER_LOG, "Enter serverClientProcessing\n");
  SSL_set_fd(ssl, SocketValue); // set connection socket to SSL state
  logData(SERVER_LOG, "serverClientProcessing after SSL_set_fd\n");
  // service connection
  const size_t bufferSize = 1024 * 16;
  char buffer[bufferSize];
  memset(buffer, 0, bufferSize);
  int read_bytes = 0;

  if (SSL_accept(ssl) == -1) // do SSL-protocol accept
  {
    ERR_print_errors_fp(stderr);
    SSL_free(ssl); // release SSL state
    close(SocketValue);
    return;
  }

  read_bytes = SSL_read(ssl, buffer, sizeof(buffer)); // get request
  logData(SERVER_LOG, "ServerClientProcessing after read_bytes\n");
  if (read_bytes <= 0) {
    SSL_free(ssl); // release SSL state
    close(SocketValue);
    return;
  }

  ParseRequest(buffer);
  logData(SERVER_LOG, "ServerClientProcessing after ParseRequest\n");
  char uri[uriSize];
  RequestUri(uri);
  logData(SERVER_LOG, "Connection from(%s), request uri (%s), method (%d) \n",
          IpAddress, uri, RequestHttpMethod());
  char ok_response[responseSize];
  char response[responseSize];

  if (RequestHttpMethod() != Http_Method_GET) {
    logData(SERVER_LOG, "Unknown type of request. Not support.\n");
    sprintf(response,
            "HTTP/1.1 501 Not implemented\r\nConnection: "
            "keep-alive\r\nContent-Type: "
            "text/html\r\nContent-Length:%d \r\n\r\n",
            0);

    int sended = 0;
    sended = SSL_write(ssl, response, (int)strlen(response));
    return;
  }

  ++requestCount;

  RequestUri(uri);
  switch (strlen(uri)) {
  case 1: // "/"
  {
    if (strcmp(uri, "/") == 0) {
      if(strlen(htmlFileData) <= chunkThreshold){
      sprintf(response,
              "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: "
              "text/html\r\nContent-Length:%d \r\n\r\n",
              (int)strlen(htmlFileData));

      int sended = 0;
      // send status OK
      sended = SSL_write(ssl, response, (int)strlen(response));
      logData(SERVER_LOG, "HTTP/1.1 200 OK - was send\n");
      if (sended > 0) {
        // send index.html
        sended = SSL_write(ssl, htmlFileData, (int)strlen(htmlFileData));
      }
    } else{
 sprintf(response,
              "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: "
              "text/html\r\nTransfer-Encoding: chunked\r\n\r\n");

      int sended = 0;
      // send status OK
      sended = SSL_write(ssl, response, (int)strlen(response));
      logData(SERVER_LOG, "HTTP/1.1 200 OK - was send\n");
      if (sended > 0) {
        // send index.html
        size_t chunckSize = 10;
        splitAndSend(htmlFileData, chunckSize);
        sended = SSL_write(ssl, htmlFileData, (int)strlen(htmlFileData));
    }
    }
    break;
  }
  case 12: // "/favicon.ico"
  {
    if (strcmp(uri, "/favicon.ico") == 0) {

      sprintf(response,
              "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: "
              "image/png\r\nContent-Length:%d \r\n\r\n",
              (int)actualIconFileSize);

      int sended = 0;
      // send status OK
      sended = SSL_write(ssl, response, (int)strlen(response));
      logData(SERVER_LOG, "Header: %s - was send\n", response);

      if (sended > 0) {
        // send favicon.png
        sended = SSL_write(ssl, iconFavBuffer, (int)actualIconFileSize);

        logData(SERVER_LOG, "favicon.png size %d was send\n",
                (int)actualIconFileSize);
      }
      break;
    }
  }
  case 38: // "/how_many_requests_were_from_beginning"
  {
    if (strcmp(uri, "/how_many_requests_were_from_beginning") == 0) {

      sprintf(response,
              "server was started:%s <br> count of GET request:%d <br>",
              startDateTime, (int)requestCount);

      sprintf(
          ok_response,
          "HTTP/1.1 200 OK\r\nConnection: "
          "keep-alive\r\nContent-Type: text/html\r\nContent-Length:%d \r\n\r\n",
          (int)strlen(response));

      int sended = 0;
      sended = SSL_write(ssl, ok_response, strlen(ok_response));
      if (sended > 0)
        sended = SSL_write(ssl, response, strlen(response));
    }

    break;
  }
  default: {
    logData(SERVER_LOG, "unknown uri requested.\n");
    sprintf(response,
            "HTTP/1.1 404 Not found\r\nConnection: keep-alive\r\nContent-Type: "
            "text/html\r\nContent-Length:%d \r\n\r\n",
            0);

    int sended = 0;
    sended = SSL_write(ssl, response, (int)strlen(response));
  }
  }
  SSL_free(ssl); // release SSL state
  close(SocketValue);
}
}
//------------------------------------------------------------------------------
// bool serverSSLInit() {
//   SSL_library_init();
//   logData(SERVER_LOG, "Enter serverSSLInit\n");
//   // load all error messages
//   SSL_load_error_strings();

//   // create new server-method instance
//   pSSLMethod = (SSL_METHOD *)DTLSv1_2_server_method();

//   // create new context from method
//   pSSLContext = SSL_CTX_new(pSSLMethod);

//   if (pSSLContext == NULL) {
//     ERR_print_errors_fp(stderr);
//     return false;
//   }

//   // set the local certificate from CertFile
//   if (SSL_CTX_use_certificate_file(pSSLContext, serverSSLCertificatePath,
//                                    SSL_FILETYPE_PEM) <= 0) {
//     ERR_print_errors_fp(stderr);
//     return false;
//   }
//   // set the private key from KeyFile (may be the same as CertFile)
//   if (SSL_CTX_use_PrivateKey_file(pSSLContext, serverSSLkeyPath,
//                                   SSL_FILETYPE_PEM) <= 0) {
//     ERR_print_errors_fp(stderr);
//     return false;
//   }

//   // verify private key
//   if (!SSL_CTX_check_private_key(pSSLContext)) {
//     ERR_print_errors_fp(stderr);
//     return false;
//   }

//   if (strlen(caPath) > 0) {
//     if (SSL_CTX_load_verify_locations(pSSLContext, caPath, NULL) == 0) {
//       ERR_print_errors_fp(stderr);
//       return false;
//     }
//   }

//   return true;
// }

void serverClientProcessing_(int SocketValue, char *IpAddress)
{

  logData(SERVER_LOG, "Enter serverClientProcessing\n");
  
  // service connection
  const size_t bufferSize = 1024 * 16;
  char buffer[bufferSize];
  memset(buffer, 0, bufferSize);
  int read_bytes = 0;
  
  ssize_t recvN = 0;
  
  recvN = read(SocketValue,buffer,bufferSize);
   
  if (recvN <= 0)
  {
    int ret_code=-1;
    logData(SERVER_LOG, "ServerClientProcessing %d read_bytes\n" , recvN);
    close(SocketValue);
    return;
  }

  ParseRequest(buffer);
  // RequestData request;
  //  memset(&request, 0, sizeof (RequestData));
  //  ParseRequest_mt(buffer, &request);
  logData(SERVER_LOG, "ServerClientProcessing after ParseRequest\n");
  char uri[uriSize];
  RequestUri(uri);
  // enum eHttpMethod resType = Http_Method_Unknown;
  // RequestHttpMethod_mt(& request, &resType);
  logData(SERVER_LOG, "Connection from(%s), request uri (%s), method (%d) \n", IpAddress, uri, RequestHttpMethod());
  char ok_response[responseSize];
  char response[responseSize];

  if (RequestHttpMethod() != Http_Method_GET)
  {
    logData(SERVER_LOG, "Unknown type of request. Not support.\n");
    sprintf(response,
            "HTTP/1.1 501 Not implemented\r\nConnection: "
            "keep-alive\r\nContent-Type: "
            "text/html\r\nContent-Length:%d \r\n\r\n",
            0);

    int sended = 0;
    sended = write(SocketValue, response, (int)strlen(response));
    return;
  }

  ++requestCount;

  RequestUri(uri);
  switch (strlen(uri))
  {
  case 1: // "/"
  {
    if (strcmp(uri, "/") == 0)
    {
      if (strlen(htmlFileData) <= chunkThreshold)
      {
        sprintf(response,
                "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: "
                "text/html\r\nContent-Length:%d \r\n\r\n",
                (int)strlen(htmlFileData));

        int sended = 0;
        // send status OK
        sended = write(SocketValue, response, (int)strlen(response));
        logData(SERVER_LOG, "HTTP/1.1 200 OK - was send\n");
        if (sended > 0)
        {
          // send index.html
          sended = write(SocketValue, htmlFileData, (int)strlen(htmlFileData));
        }
      }
    }
    else
    {
      sprintf(response,
              "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: "
              "text/html\r\nTransfer-Encoding: chunked\r\n\r\n");

      int sended = 0;
      // send status OK
      sended = write(SocketValue, response, (int)strlen(response));
      logData(SERVER_LOG, "HTTP/1.1 200 OK - was send\n");
      if (sended > 0)
      {
        // send index.html
        size_t chunckSize = 10;
        splitAndSend(htmlFileData, chunckSize);
        sended = write(SocketValue, htmlFileData, (int)strlen(htmlFileData));
      }
    }
    break;
  }
  case 12: // "/favicon.ico"
  {
    if (strcmp(uri, "/favicon.ico") == 0)
    {

      sprintf(response,
              "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: "
              "image/png\r\nContent-Length:%d \r\n\r\n",
              (int)actualIconFileSize);

      int sended = 0;
      // send status OK
      sended = write(SocketValue, response, (int)strlen(response));
      logData(SERVER_LOG, "Header: %s - was send\n", response);

      if (sended > 0)
      {
        // send favicon.png
        sended = write(SocketValue, iconFavBuffer, (int)actualIconFileSize);

        logData(SERVER_LOG, "favicon.png size %d was send\n",
                (int)actualIconFileSize);
      }
      break;
    }
  }
  default:
  {
    if (strstr(uri, "/get_date_time") != NULL)
    {
      char buf[dateTimeBuf];
      dateTime(buf);
      sprintf(response, "%s", buf);
      sprintf(
          ok_response,
          "HTTP/1.1 200 OK\r\nConnection: "
          "keep-alive\r\nContent-Type: text/html\r\nContent-Length:%d \r\n\r\n",
          (int)strlen(response));

      int sended = 0;
      sended = write(SocketValue, ok_response, strlen(ok_response));
      if (sended > 0)
        sended = write(SocketValue, response, strlen(response));
    }
    else if (strstr(uri, "/sync_time"))
    {
      char cmd[CMD_SIZE] = {};
      char value[VALUE_SIZE] = {};
      char delim[DELIM_SIZE] = {'='};
      parseLine(uri, cmd, value, delim);
      logData(SERVER_LOG, "uri cmd -%s value -%s.\n", cmd, value);
      struct timeval tp;
      tp.tv_sec = atoll(value) / 1000;
      tp.tv_usec = atoll(value) % 1000;
      int res = settimeofday(&tp, NULL);
      char buf[dateTimeBuf];
      dateTime(buf);
      sprintf(response, "%s: %s", buf , !res?"OK":"FAIL");
      sprintf(
          ok_response,
          "HTTP/1.1 200 OK\r\nConnection: "
          "keep-alive\r\nContent-Type: text/html\r\nContent-Length:%d \r\n\r\n",
          (int)strlen(response));

      int sended = 0;
      sended = write(SocketValue, ok_response, strlen(ok_response));
      if (sended > 0)
        sended = write(SocketValue, response, strlen(response));
    }
    else if (strstr(uri, "/switch"))
    {

      int fd = open("/dev/ttyS1", O_RDWR);
      char cmd[CMD_SIZE] = {};
      char value[VALUE_SIZE] = {};
      char delim[DELIM_SIZE] = {'='};
      parseLine(uri, cmd, value, delim);
      int res = 0;
      if (strcmp(value, "On") == 0)
      {
        res = write(fd, "man Onn", 7);
      }
      else if (strcmp(value, "Off") == 0)
      {
       res =  write(fd, "man Off", 7);
      }
      if (res > 0)
        sprintf(response,"switch %s:Ok", value);
      else
        sprintf(response,
                "switch %s:Fail", value);
      // sprintf(response,
      //         "server was started:%s <br> count of GET request:%d <br>",
      //         startDateTime, (int)requestCount);

      sprintf(
          ok_response,
          "HTTP/1.1 200 OK\r\nConnection: "
          "keep-alive\r\nContent-Type: text/html\r\nContent-Length:%d \r\n\r\n",
          (int)strlen(response));

      int sended = 0;
      sended = write(SocketValue, ok_response, strlen(ok_response));
      if (sended > 0)
        sended = write(SocketValue, response, strlen(response));
    } else{

    logData(SERVER_LOG, "unknown uri requested.\n");
    sprintf(response,
            "HTTP/1.1 404 Not found\r\nConnection: keep-alive\r\nContent-Type: "
            "text/html\r\nContent-Length:%d \r\n\r\n",
            0);
  
    int sended = 0;
    sended = write(SocketValue, response, (int)strlen(response));
    }
  }
}

close(SocketValue);
}

void setPathForCA(char *pathForCA) { strcpy(caPath, pathForCA); }

void setCertPath(char *aPath) { strcpy(serverSSLCertificatePath, aPath); }
//------------------------------------------------------------------------------
void setKeyPath(char *aPath) { strcpy(serverSSLkeyPath, aPath); }

