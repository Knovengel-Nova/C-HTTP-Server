# C-HTTP-Server

A small dependency-free HTTP/1.1 static file server written in C.

## Build and run

```sh
make
./server                 # listens on http://localhost:8080
./server 9000            # use a different port
```

The server serves files from `www/`. It supports `GET` and `HEAD`, sends
appropriate common MIME types, handles malformed/unsupported requests, and
rejects path traversal attempts. Stop it with `Ctrl-C`.

Example:

```sh
curl -i http://localhost:8080/
curl -I http://localhost:8080/style.css
```