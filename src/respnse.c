#include <stdio.h>
#include "response.h"
#include "server.h"

//  private APIs
static const char *statusString(HttpStatus status);
static void initResponse(HttpResponse *resp);
static void addHeader(HttpResponse *resp, const char *name, const char *value);
static void sendResponse(Client *client, const HttpResponse *response);
static void sendStatusPage(Client *Client, HttpStatus status, const char *title);

void sendHTML(Client *client, HttpStatus status, const char *html)
{
    HttpResponse resp;

    initResponse(&resp);

    resp.status = status;
    resp.body = html;
    resp.bodyLength = strlen(html);
    addHeader(&resp, "Content-Type", "text/html");
    addHeader(&resp, "Connection", "close");
    sendResponse(client, &resp);
}

void sendText(Client *client, HttpStatus status, const char *text)
{
    HttpResponse resp;

    initResponse(&resp);

    resp.status = status;
    resp.body = text;
    resp.bodyLength = strlen(text);
    addHeader(&resp, "Content-Type", "text/plain");
    addHeader(&resp, "Connection", "close");
    sendResponse(client, &resp);
}

void sendJSON(Client *client, HttpStatus status, const char *json)
{
    HttpResponse resp;

    initResponse(&resp);

    resp.status = status;
    resp.body = json;
    resp.bodyLength = strlen(json);
    addHeader(&resp, "Content-Type", "application/json");
    addHeader(&resp, "Connection", "close");
    sendResponse(client, &resp);
}

void sendBinary(Client *client, HttpStatus status, const void *data, size_t size, const char *mime)
{
    HttpResponse resp;

    initResponse(&resp);

    resp.status = status;
    resp.body = data;
    resp.bodyLength = size;
    addHeader(&resp, "Content-Type", mime);
    addHeader(&resp, "Connection", "close");
    sendResponse(client, &resp);
}

void redirect(Client *client, const char *location)
{
    HttpResponse resp;
}

void sendError(Client *client, HttpStatus status)
{
    char html[512];

    snprintf(html,
             sizeof(html),
             "<!DOCTYPE html>"
             "<html>"
             "<head>"
             "<title>%d %s</title>"
             "</head>"
             "<body>"
             "<h1>%d %s</h1>"
             "</body>"
             "</html>",
             status,
             statusString(status),
             status,
             statusString(status));

    sendHTML(client,
             status,
             html);
}

static void sendStatusPage(Client *client, HttpStatus status, const char *title)
{
    char html[256];

    snprintf(html,
             sizeof(html),
             "<html>"
             "<head><title>%d %s</title></head>"
             "<body><h1>%d %s</h1></body>"
             "</html>",
             status,
             title,
             status,
             title);

    sendHTML(client, status, html);
}

static const char *statusString(HttpStatus status)
{
    switch (status)
    {
    /* 1xx */
    case HTTP_CONTINUE:
        return "Continue";

    case HTTP_SWITCHING_PROTOCOLS:
        return "Switching Protocols";

    /* 2xx */
    case HTTP_OK:
        return "OK";

    case HTTP_CREATED:
        return "Created";

    case HTTP_ACCEPTED:
        return "Accepted";

    case HTTP_NON_AUTHORITATIVE_INFORMATION:
        return "Non-Authoritative Information";

    case HTTP_NO_CONTENT:
        return "No Content";

    case HTTP_RESET_CONTENT:
        return "Reset Content";

    case HTTP_PARTIAL_CONTENT:
        return "Partial Content";

    /* 3xx */
    case HTTP_MULTIPLE_CHOICES:
        return "Multiple Choices";

    case HTTP_MOVED_PERMANENTLY:
        return "Moved Permanently";

    case HTTP_FOUND:
        return "Found";

    case HTTP_SEE_OTHER:
        return "See Other";

    case HTTP_NOT_MODIFIED:
        return "Not Modified";

    case HTTP_USE_PROXY:
        return "Use Proxy";

    case HTTP_UNUSED:
        return "Unused";

    case HTTP_TEMPORARY_REDIRECT:
        return "Temporary Redirect";

    /* 4xx */
    case HTTP_BAD_REQUEST:
        return "Bad Request";

    case HTTP_UNAUTHORIZED:
        return "Unauthorized";

    case HTTP_PAYMENT_REQUIRED:
        return "Payment Required";

    case HTTP_FORBIDDEN:
        return "Forbidden";

    case HTTP_NOT_FOUND:
        return "Not Found";

    case HTTP_METHOD_NOT_ALLOWED:
        return "Method Not Allowed";

    case HTTP_NOT_ACCEPTABLE:
        return "Not Acceptable";

    case HTTP_PROXY_AUTHENTICATION_REQUIRED:
        return "Proxy Authentication Required";

    case HTTP_REQUEST_TIMEOUT:
        return "Request Timeout";

    case HTTP_CONFLICT:
        return "Conflict";

    case HTTP_GONE:
        return "Gone";

    case HTTP_LENGTH_REQUIRED:
        return "Length Required";

    case HTTP_PRECONDITION_FAILED:
        return "Precondition Failed";

    case HTTP_REQUEST_ENTITY_TOO_LARGE:
        return "Request Entity Too Large";

    case HTTP_REQUEST_URL_TOO_LONG:
        return "Request-URI Too Long";

    case HTTP_UNSUPPORTED_MEDIA_TYPE:
        return "Unsupported Media Type";

    case HTTP_REQUEST_RANGE_NOT_SATISFIABLE:
        return "Requested Range Not Satisfiable";

    case HTTP_EXPECTATION_FAILED:
        return "Expectation Failed";

    /* 5xx */
    case HTTP_INTERNAL_SERVER_ERROR:
        return "Internal Server Error";

    case HTTP_NOT_IMPLEMENTED:
        return "Not Implemented";

    case HTTP_BAD_GATEWAY:
        return "Bad Gateway";

    case HTTP_SERVICE_UNAVAILABLE:
        return "Service Unavailable";

    case HTTP_GATEWAY_TIMEOUT:
        return "Gateway Timeout";

    case HTTP_HTTP_VERSION_NOT_SUPPORTED:
        return "HTTP Version Not Supported";

    default:
        return "Unknown";
    }
}

static void initResponse(HttpResponse *resp)
{
    resp->status = HTTP_OK;
    resp->body = NULL;
    resp->bodyLength = 0;
    resp->headerCount = 0;
}

static void addHeader(HttpResponse *resp, const char *name, const char *value)
{
    if (resp->headerCount >= MAX_HEADERS)
        return;

    HttpHeader header;
    strcpy(resp->headers[resp->headerCount].name, name);
    strcpy(resp->headers[resp->headerCount].value, value);

    resp->headerCount++;
}

static void sendResponse(Client *client, const HttpResponse *response)
{
    char buffer[4096];
    int len = 0;

    // status line
    len += snprintf(buffer + len,
                    sizeof(buffer) - len,
                    "HTTP/1.1 %d %s\r\n",
                    response->status,
                    statusString(response->status));

    // headers
    for (int i = 0; i < response->headerCount; i++)
    {
        len += snprintf(buffer + len,
                        sizeof(buffer) - len,
                        "%s: %s\r\n",
                        response->headers[i].name,
                        response->headers[i].value);
    }

    // content length
    len += snprintf(buffer + len, sizeof(buffer) - len, "Content-Length: %zu", response->bodyLength);

    // empty line
    len += snprintf(buffer + len, sizeof(buffer) - len, "\r\n");

    // send headers
    send(client->fd, buffer, len, 0);

    // body
    if (response->body != NULL && response->bodyLength > 0)
    {
        send(client->fd, response->body, response->bodyLength, 0);
    }
}