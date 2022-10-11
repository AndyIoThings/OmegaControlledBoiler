#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include "../pre_header.h"

// GET /favicon.ico HTTP

enum eHttpMethod {
  Http_Method_Unknown = 0,
  // The GET method is used to retrieve information from the given server
  // using a given URI. Requests using GET should only retrieve data and
  // should have no other effect on the data.
  Http_Method_GET
};

typedef struct RequestData{
char *rawData;  
char *uri;
char *httpMethod;
enum eHttpMethod httpMethodType;
bool isChunkedTransferEnabled;
bool isKeepAliveEnabled;
} RequestData; 

void parseLine(const char *buf, char*cmd, char *value, const char *delim);
void ParseRequest(const char *);
void ParseRequest_mt(const char *, RequestData *);

const enum eHttpMethod RequestHttpMethod(void);
void RequestUri(char *uri);

void RequestHttpMethod_mt(RequestData* request, enum eHttpMethod methodType);
void RequestUri_mt(RequestData* request, char *uri);

#endif // REQUEST_PARSER_HPP