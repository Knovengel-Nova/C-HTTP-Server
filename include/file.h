#ifndef FILE_H
#define FILE_H
typedef struct Client Client;

int serveFile(Client *client, const char *root, const char *urlPath, int headOnly);
#endif
