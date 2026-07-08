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
    //  1xx: Informational
    HTTP_CONTINUE                       =   100,
    HTTP_SWITCHING_PROTOCOLS            =   101,

    //  2xx: Success
    HTTP_OK                             =   200,
    HTTP_CREATED                        =   201,
    HTTP_ACCEPTED                       =   202,
    HTTP_NON_AUTHORITATIVE_INFORMATION  =   203,
    HTTP_NO_CONTENT                     =   204,
    HTTP_RESET_CONTENT                  =   205,
    HTTP_PARTIAL_CONTENT                =   206,

    //  3xx: Redirection
    HTTP_MULTIPLE_CHOICES               =   300,
    HTTP_MOVED_PERMANENTLY              =   301,
    HTTP_FOUND                          =   302,
    HTTP_SEE_OTHER                      =   303,
    HTTP_NOT_MODIFIED                   =   304,
    HTTP_USE_PROXY                      =   305,
    HTTP_UNUSED                         =   306,
    HTTP_TEMPORARY_REDIRECT             =   307,

    //  4xx: Client Error
    HTTP_BAD_REQUEST                    =   400,
    HTTP_UNAUTHORIZED                   =   401,
    HTTP_PAYMENT_REQUIRED               =   402,
    HTTP_FORBIDDEN                      =   403,
    HTTP_NOT_FOUND                      =   404,
    HTTP_METHOD_NOT_ALLOWED             =   405,
    HTTP_NOT_ACCEPTABLE                 =   406,
    HTTP_PROXY_AUTHENTICATION_REQUIRED  =   407,
    HTTP_REQUEST_TIMEOUT                =   408,
    HTTP_CONFLICT                       =   409,
    HTTP_GONE                           =   410,
    HTTP_LENGTH_REQUIRED                =   411,
    HTTP_PRECONDITION_FAILED            =   412,
    HTTP_REQUEST_ENTITY_TOO_LARGE       =   413,
    HTTP_REQUEST_URL_TOO_LONG            =   414,
    HTTP_UNSUPPORTED_MEDIA_TYPE         =   415,
    HTTP_REQUEST_RANGE_NOT_SATISFIABLE  =   416,
    HTTP_EXPECTATION_FAILED             =   417,

    //  5xx: Server Error
    HTTP_INTERNAL_SERVER_ERROR          =   500,
    HTTP_NOT_IMPLEMENTED                =   501,
    HTTP_BAD_GATEWAY                    =   502,
    HTTP_SERVICE_UNAVAILABLE            =   503,
    HTTP_GATEWAY_TIMEOUT                =   504,
    HTTP_HTTP_VERSION_NOT_SUPPORTED     =   505

} HttpStatus;

typedef struct
{
    char name[MAX_HEADER_NAME];
    char value[MAX_HEADER_VALUE];
} HttpHeader;

#endif
