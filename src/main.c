#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include "server.h"
#include "request.h"
#include "response.h"
#include "router.h"
#include "file.h"

#define DEFAULT_PORT 8080
#define REQUEST_BUFFER_SIZE 16384

static volatile sig_atomic_t running = 1;

static void stopRunning(int signalNumber)
{
    (void)signalNumber;
    running = 0;
}

int main(int argc, char **argv)
{
    int port = DEFAULT_PORT;
    if (argc > 2 ||
        (argc == 2 && ((port = atoi(argv[1])) <= 0 || port > 65535))) {
        fprintf(stderr, "Usage: %s [port]\\n", argv[0]);
        return EXIT_FAILURE;
    }
    signal(SIGINT, stopRunning);
    signal(SIGTERM, stopRunning);

    Server server = startServer(port);
    printf("HTTP server running on http://localhost:%d\\n", port);

    while (running)
    {
        Client client = acceptServer(&server);
        char buffer[REQUEST_BUFFER_SIZE];
        ssize_t bytes;
        if (client.fd < 0)
            continue;
        bytes = recv(client.fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            close(client.fd);
            continue;
        }
        buffer[bytes] = '\0';
        int parseResult = parseRequest(buffer, &client.request);
        if (parseResult == -2)
            sendError(&client, HTTP_HTTP_VERSION_NOT_SUPPORTED);
        else if (parseResult != 0)
            sendError(&client, HTTP_BAD_REQUEST);
        else if (strcmp(client.request.method, "GET") != 0 &&
                 strcmp(client.request.method, "HEAD") != 0)
            sendError(&client, HTTP_METHOD_NOT_ALLOWED);
        else {
            char path[1024];
            char *query;
            snprintf(path, sizeof(path), "%s", client.request.path);
            query = strchr(path, '?');
            if (query != NULL)
                *query = '\0';
            if (!serveFile(&client, "www", path,
                           strcmp(client.request.method, "HEAD") == 0))
                sendError(&client, HTTP_NOT_FOUND);
        }
        freeRequest(&client.request);
        close(client.fd);
    }

    stopServer(server);

    return 0;
}
