#include "../RequestHandler/requestParser.h"
#include "../Logger/logger.h"
#include "../ProtocolHelpers/HttpProtocolHelper.h"
// add request struct to store request properties
//------------------------------------------------------------------------------
void parseRequestLine(char *buf);
void parseRequestLine_mt(const char *__restrict buf, RequestData *request);
void parseRequestFirstLine_mt(const char *__restrict buf,
                              RequestData *request);
void parseRequestNotFirstLine_mt(const char *__restrict buf,
                                 RequestData *request);
enum eUriSize { URI_SIZE = 1024 };
static enum eHttpMethod httpMethodType = Http_Method_Unknown;
static char uriName[URI_SIZE];

void ParseRequest(const char *__restrict buf) {
  logData(SERVER_LOG, "buf is %s.\n", buf);
  const char *delim = "\r\n";
  char *token = strtok((char *)buf, delim);
  logData(SERVER_LOG, "token is %s.\n", token);
  size_t i = 1;
  while (token) {

    if (1 == i)
      parseRequestLine(token);
    token = strtok(NULL, delim);
    ++i;
  }
}
//------------------------------------------------------------------------------
void parseLine(const char *buf, char*cmd, char *value, const char *delim) {
  const size_t methodSize = 512;
  //const char *delim = " ";
  char httpMethodName[methodSize];
  logData(SERVER_LOG, "parseLine:buf is %s.\n", buf);
  char *token = strtok(buf, delim);
  size_t i = 0;
  logData(SERVER_LOG, "parseLine:token is %s.\n", token);
  while (token) {

    if (0 == i)
      strcpy(cmd, token);
    else if (1 == i)
      strcpy(value, token);

    token = strtok(NULL, delim);
    ++i;
  }
  logData(SERVER_LOG, "command is %s, size is %d.\n", cmd,
          strlen(cmd));
}
//------------------------------------------------------------------------------
void parseRequestLine(char *buf) {
  const size_t methodSize = 512;
  const char *delim = " ";
  char httpMethodName[methodSize];
  logData(SERVER_LOG, "parseRequestLine:buf is %s.\n", buf);
  char *token = strtok(buf, delim);
  size_t i = 0;
  logData(SERVER_LOG, "parseRequestLine:token is %s.\n", token);
  while (token) {

    if (0 == i)
      strcpy(httpMethodName, token);
    else if (1 == i)
      strcpy(uriName, token);

    token = strtok(NULL, delim);
    ++i;
  }
  logData(SERVER_LOG, "method is %s, size is %d.\n", httpMethodName,
          strlen(httpMethodName));
  switch (strlen(httpMethodName)) {
  case 3: // GET
  {
    if (0 == strcmp(httpMethodName, "GET")) {
      httpMethodType = Http_Method_GET;
      logData(SERVER_LOG, "case 3 is %s, type is %d.\n", httpMethodName,
              httpMethodType);
    }
    break;
  }
  default: {
    httpMethodType = Http_Method_Unknown;
  }
  }
}
//------------------------------------------------------------------------------
const enum eHttpMethod RequestHttpMethod(void) {

  enum eHttpMethod ret = httpMethodType;

  return ret;
}
//------------------------------------------------------------------------------
void RequestUri(char *uri) { strcpy(uri, uriName); }
//------------------------------------------------------------------------------

void ParseRequest_mt(const char *__restrict buf, RequestData *request) {

  if (!buf || !request)
    logData(SERVER_LOG, "ParseRequest_mt buf or request is NULL.\n");
  return;
  logData(SERVER_LOG, "ParseRequest_mt buf is %s.\n", buf);
  strcpy(request->rawData, buf);
  const char *delim = "\r\n";
  char *token = strtok((char *)buf, delim);
  logData(SERVER_LOG, "ParseRequest_mt token is %s.\n", token);
  size_t i = 1;
  while (token) {

    if (1 == i)
      parseRequestFirstLine_mt(token, request);
    else
      parseRequestNotFirstLine_mt(token, request);

    token = strtok(NULL, delim);
    ++i;
  }
}
//-----------------------------------------------------------------------------
void parseRequestFirstLine_mt(const char *__restrict buf,
                              RequestData *request) {

  if (!buf || !request) {
    logData(SERVER_LOG, "ParseRequest_mt buf or request is NULL.\n");
    return;
  }
  const size_t methodSize = 512;
  const char *delim = " ";

  logData(SERVER_LOG, "parseRequestLine:buf is %s.\n", buf);
  char *token = strtok(buf, delim);
  size_t i = 0;
  logData(SERVER_LOG, "parseRequestLine:token is %s.\n", token);

  while (token) {

    if (0 == i)
      strcpy(request->httpMethod, token);
    else if (1 == i)
      strcpy(request->uri, token);

    token = strtok(NULL, delim);
    ++i;
  }
  logData(SERVER_LOG, "method is %s, size is %d.\n", request->httpMethod,
          strlen(request->httpMethod));
  switch (strlen(request->httpMethod)) {
  case 3: // GET
  {
    if (0 == strcmp(request->httpMethod, "GET")) {
      request->httpMethodType = Http_Method_GET;
      logData(SERVER_LOG, "case 3 is %s, type is %d.\n", request->httpMethod,
              request->httpMethodType);
    }
    break;
  }

  default: {
    request->httpMethodType = Http_Method_Unknown;
  }
  }
}
//-----------------------------------------------------------------------------
void parseRequestNotFirstLine_mt(const char *__restrict buf,
                                 RequestData *request) {

  if (!buf || !request) {
    logData(SERVER_LOG, "ParseRequest_mt buf or request is NULL.\n");
    return;
  }
  const char *delim = ":";
  bool connectionField = false;
  logData(SERVER_LOG, "parseRequestLine:buf is %s.\n", buf);
  char *token = strtok(buf, delim);
  size_t i = 0;

  logData(SERVER_LOG, "parseRequestLine:token is %s.\n", token);

  while (token) {

    if (0 == i)
      if (0 == strcmp(token, HTTP_H_CONNECTION))
       connectionField = true;

    if (1 == i && connectionField) {
      int startFrom = strstr(token, HTTP_H_CONNECTION_KEEP_ALIVE);
      char newToken[256];
      strncpy(newToken, token, strlen(token));
      if (0 == strcmp(HTTP_H_CONNECTION_KEEP_ALIVE, newToken)) {
        request->isKeepAliveEnabled = true;
      }
    }
    token = strtok(NULL, delim);
    ++i;
  }
  logData(SERVER_LOG, "method is %s, size is %d.\n", request->httpMethod,
          strlen(request->httpMethod));
}
//--------------------------------------------------------------------------

void RequestHttpMethod_mt(RequestData *request, enum eHttpMethod methodType) {

  methodType = request->httpMethodType;
}
void RequestUri_mt(RequestData *request, char *uri) {

  strcpy(uri, request->httpMethod);
}
