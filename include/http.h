#include <stdlib.h>

#ifndef HTTP_H
#define HTTP_H

#define MAX_METHOD_LENGTH    8
#define MAX_PATH_LENGTH      1024
#define MAX_VERSION_LENGTH   16
#define MAX_HEADERS          64
#define MAX_HEADER_NAME      64
#define MAX_HEADER_VALUE     512

typedef enum
{
    HTTP_OK = 200,
    HTTP_CREATED = 201,

    HTTP_MOVED_PERMANENTLY = 301,

    HTTP_BAD_REQUEST = 400,
    HTTP_UNAUTHORIZED = 401,
    HTTP_FORBIDDEN = 403,
    HTTP_NOT_FOUND = 404,
    HTTP_METHOD_NOT_ALLOWED = 405,

    HTTP_INTERNAL_SERVER_ERROR = 500
} HttpStatus;

typedef struct
{
    char name[MAX_HEADER_NAME];
    char value[MAX_HEADER_VALUE];
} HttpHeader;

#endif
