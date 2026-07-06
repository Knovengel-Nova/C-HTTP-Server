#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 2048
#define PORT 80

// listen for requests on port 1708
// give them whatever they request for

void freeResources();
void processRequest(char* request);
void get(char path[256]);
void sendFavicon();

int server_fd, client_fd;

int main(){
    printf("Hello World!\n");
    
    struct sockaddr_in server_addr;
    socklen_t addr_len  = sizeof(server_addr);
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd == -1){
        perror("Server Socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // listen (fd, no of connections in queue next ones are refused)
    if(listen(server_fd , 5) < 0){
        perror("Listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("HTTP server running on http://localhost:%d\n", PORT);

    while(1){
        client_fd = accept(server_fd, (struct sockaddr*)&server_addr, &addr_len);

        

        if(client_fd < 0){
            perror("Client Accept");
            continue;
        }

        int bytes = read(client_fd, buffer, BUFFER_SIZE-1);
        if(bytes > 0){
            buffer[bytes] = '\0';
            processRequest(buffer);
        }
        close(client_fd);

        

    }

    close(server_fd);

    return 0;
}

void processRequest(char* request){
    printf("Request:\n%s\n", request);

    char method[16];
    char path[256];
    char version[16];

    sscanf(request, "%15s %255s %15s", method, path, version);
    if(strcmp(method, "GET") == 0){
        get(path);
    }

}

void get(char path[256]){
    if(strcmp(path, "/favicon.ico") == 0){
        sendFavicon();
        return;
    }

    char* response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<html><body>"
        "<h1>Hello From Server<h1>"
        "</body></html>";

    write(client_fd, response, strlen(response));
}

void sendFavicon(){
    FILE *file = fopen("favicon.ico", "rb");

    if (file == NULL) {
        // Send a 404 response
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char header[256];

    sprintf(header,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: image/x-icon\r\n"
        "Content-Length: %ld\r\n"
        "Connection: close\r\n"
        "\r\n",
        size);

    send(client_fd, header, strlen(header), 0);

    char buffer[4096];
    size_t bytes;

    while ((bytes = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(client_fd, buffer, bytes, 0);
    }

    fclose(file);

    return;
}

void freeResources(){
    close(server_fd);
    close(client_fd);
}
