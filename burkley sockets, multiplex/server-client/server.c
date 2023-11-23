
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

//strace ./server - все системные вызовы
//strace ./server 2> ./server.log - записать в файл
//ps aux | fgrep server - список открытых дескрипторов
//lsof -p 3343 - список дескрипторов на данном сервере
//strace -p 3343 - инфа по процессу.

/*
int set_nonblock(int fd)
{
    int flags;
#if defined(O_NONBLOCK)
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif
} 
*/

int main(int argc, char **argv) {
    int masterSocket = socket(
        AF_INET, //IPv4
        SOCK_STREAM, //TCP
        IPPROTO_TCP);
    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(12345);
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY); //0.0.0.0
    bind(masterSocket, (struct sockaddr *)(&sockAddr), sizeof(sockAddr));

    listen(masterSocket, SOMAXCONN);

    while(1)
    {
        int slaveSocket = accept(masterSocket, 0, 0);
        char buf[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        recv(slaveSocket, buf, 10, MSG_NOSIGNAL);
        send(slaveSocket, "OK!", 3, MSG_NOSIGNAL);
        shutdown(slaveSocket, SHUT_RDWR);
        close(slaveSocket);

        printf("%s\n", buf);
        //telnet 127.0.0.1 12345 
        //PING

    } 
    
    return 0;
}