#include <stdbool.h>
#include <netinet/in.h>
#include "request.h"

#ifndef SERVER_H
#define SERVER_H

typedef struct
{
    int fd;
    int port;
    struct sockaddr_in address;
} Server;

typedef struct Client
{
    int fd;
    struct sockaddr_in address;
    bool keepAlive;
    HttpRequest request;
} Client;

Server startServer(int port);
Client acceptServer(const Server *server);
void stopServer(Server server);

#endif
