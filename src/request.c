#include "request.h"

#include <stdio.h>

int parseRequest(char *buffer, HttpRequest *request)
{
    return sscanf(
        buffer,
        "%15s %15s %255s",
        request->method,
        request->path,
        request->version);
}
