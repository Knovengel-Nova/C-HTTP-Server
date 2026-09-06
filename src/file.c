#include "file.h"
#include "mime.h"
#include "response.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int serveFile(Client *client, const char *root, const char *urlPath, int headOnly)
{
    char path[2048];
    struct stat info;
    FILE *file;
    unsigned char *body = NULL;
    size_t size;

    if (urlPath[0] != '/' || strstr(urlPath, "..") != NULL ||
        strchr(urlPath, '\\') != NULL || strlen(urlPath) >= 1024)
        return 0;
    if (strcmp(urlPath, "/") == 0)
        urlPath = "/index.html";
    if (snprintf(path, sizeof(path), "%s/%s", root, urlPath + 1) >= (int)sizeof(path))
        return 0;
    if (stat(path, &info) != 0 || !S_ISREG(info.st_mode) || info.st_size < 0)
        return 0;
    file = fopen(path, "rb");
    if (file == NULL)
        return 0;
    size = (size_t)info.st_size;
    if (!headOnly && size > 0) {
        body = malloc(size);
        if (body == NULL || fread(body, 1, size, file) != size) {
            free(body);
            fclose(file);
            return 0;
        }
    }
    sendBinary(client, HTTP_OK, body, size, mimeTypeForPath(path));
    free(body);
    fclose(file);
    return 1;
}