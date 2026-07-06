#ifndef RESPONSE_H
#define RESPONSE_H

typedef struct
{
    HttpStatus status;

    HttpHeader headers[MAX_HEADERS];
    int headerCount;

    const void *body;
    size_t bodyLength;

    char contentType[64];

} HttpResponse;

void sendHTML(Client *client, HttpStatus status, const char *html);

void sendText(Client *client, HttpStatus status, const char *text);

void sendJSON(Client *client, HttpStatus status, const char *json);

void sendBinary(Client *client, HttpStatus status, const void *data, size_t size, const char *mime);

void redirect(Client *, const char *location);


//  errors
void send400(Client *);

void send401(Client *);

void send403(Client *);

void send404(Client *);

void send405(Client *);

void send500(Client *);

#endif
