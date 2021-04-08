#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <alloca.h>
#include <unistd.h>
#include <arpa/inet.h>

int
main(int argc, char* argv[])
{
    assert(argc == 3);
    int rv, sock;
    char* buf = alloca(256);

    const char* host = argv[1];
    const char* path = argv[2];

    struct addrinfo* hints = alloca(sizeof(struct addrinfo));
    memset(hints, 0, sizeof(struct addrinfo));
    hints->ai_family   = AF_UNSPEC;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_flags    = 0;
    hints->ai_protocol = 0;

    struct addrinfo* addrs = alloca(sizeof(struct addrinfo));
    rv = getaddrinfo(host, "80", hints, &addrs);
    if (rv != 0) {
        printf("error: %s\n", gai_strerror(rv));
        return 1;
    }

    struct addrinfo* ai;
    for (ai = addrs; ai != 0; ai->ai_next) {
        getnameinfo(ai->ai_addr, ai->ai_addrlen, buf, 256, 0, 0, NI_NUMERICHOST);
        printf("Addr: %s\n", buf);

        sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (sock == -1)
            continue;

        rv = connect(sock, ai->ai_addr, ai->ai_addrlen);
        if (rv != -1)
            break;

        close(sock);
    }

    freeaddrinfo(addrs);

    snprintf(buf, 256, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, host);
    rv = send(sock, buf, strlen(buf), 0);
    assert(rv != -1);

    rv = 1;
    while (rv) {
        rv = recv(sock, buf, 256, 0);
        assert(rv != -1);
        fwrite(buf, 1, rv, stdout);
    }

    close(sock);

    return 0;
}

