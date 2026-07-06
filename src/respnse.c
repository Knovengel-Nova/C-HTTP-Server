#include "response.h"
#include "server.h"

void sendHTML(Client *client, HttpStatus status, const char *html){

}

void sendText(Client *client, HttpStatus status, const char *text){

}

void sendJSON(Client *client, HttpStatus status, const char *json){

}

void sendBinary(Client *client, HttpStatus status, const void *data, size_t size, const char* mime){

}

void redirect(Client* client, const char *location){

}

void send400(Client *client){
    
}

void send401(Client *client){
    
}

void send403(Client *client){
    
}

void send404(Client *client){
    
}

void send405(Client *client){
    
}

void send500(Client *client){
    
}
