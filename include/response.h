#include "http.h"
#ifndef RESPONSE_H
#define RESPONSE_H

typedef struct
{
    HttpStatus status;

    HttpHeader headers[MAX_HEADERS];
    int headerCount;

    const void *body;
    size_t bodyLength;

} HttpResponse;

//  public APIs
void sendHTML(Client *client, HttpStatus status, const char *html);

void sendText(Client *client, HttpStatus status, const char *text);

void sendJSON(Client *client, HttpStatus status, const char *json);

void sendBinary(Client *client, HttpStatus status, const void *data, size_t size, const char *mime);

void redirect(Client *, const char *location);

void sendError(Client *client, HttpStatus status);

#endif
