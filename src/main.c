#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "server.h"
#include "request.h"
#include "response.h"
#include "router.h"
#include "file.h"

#define PORT 80

int main(){
    Server server;
    server = startServer(PORT);
    
    while(1){
        Client client;
        client = acceptServer(server);

        //read request
        //parse request
        //route request

        close(client.fd);


    }
    
    stopServer(server);

    return 0;
}
