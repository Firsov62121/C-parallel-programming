
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv) {
    int Socket = socket(
        AF_INET, //IPv4
        SOCK_STREAM, //TCP
        IPPROTO_TCP);
    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(12345);
    sockAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //127.0.0.1
    
    connect(Socket, (struct sockaddr *)(&sockAddr), sizeof(sockAddr));

    char buf[] = "Hello!";
    send(Socket, buf, 6, MSG_NOSIGNAL);
    recv(Socket, buf, 4, MSG_NOSIGNAL);

    shutdown(Socket, SHUT_RDWR);
    close(Socket);

    printf("%s\n", buf);
    
    return 0;
}