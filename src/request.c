#include "request.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

void initRequest(HttpRequest *request)
{
    memset(request, 0, sizeof(*request));
}

int parseRequest(const char *buffer, HttpRequest *request)
{
    const char *lineEnd;
    const char *cursor;
    char line[2048];

    initRequest(request);
    lineEnd = strstr(buffer, "\r\n");
    if (lineEnd == NULL || (size_t)(lineEnd - buffer) >= sizeof(line))
        return -1;
    memcpy(line, buffer, (size_t)(lineEnd - buffer));
    line[lineEnd - buffer] = '\0';
    if (sscanf(line, "%7s %1023s %15s", request->method, request->path,
               request->version) != 3)
        return -1;
    if (strcmp(request->version, "HTTP/1.0") != 0 &&
        strcmp(request->version, "HTTP/1.1") != 0)
        return -2;

    cursor = lineEnd + 2;
    while (!(cursor[0] == '\r' && cursor[1] == '\n')) {
        const char *end = strstr(cursor, "\r\n");
        const char *colon;
        size_t nameLength;
        size_t valueLength;
        if (end == NULL || (size_t)(end - cursor) >= 1024)
            return -1;
        colon = memchr(cursor, ':', (size_t)(end - cursor));
        if (colon == NULL || request->headerCount >= MAX_HEADERS)
            return -1;
        nameLength = (size_t)(colon - cursor);
        while (nameLength > 0 && isspace((unsigned char)cursor[nameLength - 1]))
            nameLength--;
        if (nameLength == 0 || nameLength >= MAX_HEADER_NAME)
            return -1;
        memcpy(request->headers[request->headerCount].name, cursor, nameLength);
        request->headers[request->headerCount].name[nameLength] = '\0';
        cursor = colon + 1;
        while (*cursor == ' ' || *cursor == '\t')
            cursor++;
        valueLength = (size_t)(end - cursor);
        if (valueLength >= MAX_HEADER_VALUE)
            return -1;
        memcpy(request->headers[request->headerCount].value, cursor, valueLength);
        request->headers[request->headerCount].value[valueLength] = '\0';
        request->headerCount++;
        cursor = end + 2;
    }
    return 0;
}

const char *getHeader(const HttpRequest *request, const char *name)
{
    for (int i = 0; i < request->headerCount; i++)
        if (strcasecmp(request->headers[i].name, name) == 0)
            return request->headers[i].value;
    return NULL;
}

void freeRequest(HttpRequest *request)
{
    free(request->body);
    request->body = NULL;
    request->bodyLength = 0;
}
