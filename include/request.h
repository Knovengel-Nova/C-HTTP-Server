#include "http.h"

#ifndef REQUEST_H
#define REQUEST_H

typedef struct
{
    char method[MAX_METHOD_LENGTH];
    char path[MAX_PATH_LENGTH];
    char version[MAX_VERSION_LENGTH];

    HttpHeader headers[MAX_HEADERS];
    int headerCount;

    char *body;
    size_t bodyLength;

} HttpRequest;

typedef struct
{
    char key[64];
    char value[256];
} QueryParam;

typedef struct
{
    char name[64];
    char value[256];
} Cookie;

void initRequest(HttpRequest *req);

int parseRequest(const char *buffer, HttpRequest *req);

const char *getHeader(const HttpRequest *req, const char *name);

void freeRequest(HttpRequest *req);

#endif
