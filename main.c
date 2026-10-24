#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>

int main(int argc, char **argv) {
    const char *host = argc > 1 ? argv[1] : "example.com";
    struct addrinfo hints = {0}, *res;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host, "80", &hints, &res)) { puts("DNS falhou"); return 1; }

    int s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (connect(s, res->ai_addr, res->ai_addrlen)) { puts("Conexao falhou"); return 1; }

    char req[256];
    snprintf(req, sizeof req,
        "GET / HTTP/1.0\r\nHost: %s\r\nUser-Agent: MeuNavegador\r\n\r\n", host);
    write(s, req, strlen(req));

    char buf[1024]; int n;
    while ((n = read(s, buf, sizeof buf - 1)) > 0) { buf[n] = 0; fputs(buf, stdout); }
    close(s);
    return 0;
}
