#include "server.h"
#include <arpa/inet.h>

Server startServer(int port)
{
    Server server;

    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1)
    {
        perror("Server Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(server_fd, (struct sockaddr *)&server_addr, addr_len) < 0)
    {
        perror("Bind Socket");
        close(server_fd);
        exit(1);
    }

    if (listen(server_fd, 5) < 0)
    {
        perror("Listen");
        close(server_fd);
        exit(1);
    }

    server.fd = server_fd;
    server.port = port;
    server.address = server_addr;

    return server;
}

Client acceptServer(Server server)
{
    Client client;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(server.fd, (struct sockaddr *)&client_addr, &client_len);

    if (client_fd < 0)
    {
        perror("accept");
    }

    client.fd = client_fd;
    client.address = client_addr;

    return client;
}

void stopServer(Server server)
{
    close(server.fd);
}
