
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <cerrno>
// Unix OS
#define MAX_EVENTS 32

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

int main(int argc, char **argv) {
    int masterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(12345);
    sockAddr.sin_addr.s_addr = htons(INADDR_ANY);
    bind(masterSocket, (struct sockaddr *)(&sockAddr), sizeof(sockAddr));
    
    set_nonblock(masterSocket);

    listen(masterSocket, SOMAXCONN);

    int Epoll = epoll_create1(0);
    struct epoll_event event;
    event.data.fd = masterSocket;
    event.events = EPOLLIN;
    epoll_ctl(Epoll, EPOLL_CTL_ADD, masterSocket, &event); //регистрация события

    while(true)
    {
        struct epoll_event events[MAX_EVENTS];
        int N = epoll_wait(Epoll, events, MAX_EVENTS, -1); // -1 - вечное ожидания
        for(uint i = 0; i < N; ++i)
        {
            if(events[i].data.fd == masterSocket)
            {
                int slaveSocket = accept(masterSocket, 0, 0);
                set_nonblock(slaveSocket);
                struct epoll_event event;
                event.data.fd = slaveSocket;
                event.events = EPOLLIN;
                epoll_ctl(Epoll, EPOLL_CTL_ADD, slaveSocket, &event);
            }
            else
            {
                static char buf[1024];
                int recvRes = recv(events[i].data.fd, buf, 1024, MSG_NOSIGNAL);
                if(recvRes == 0 && errno != EAGAIN)
                {
                    shutdown(events[i].data.fd, SHUT_RDWR);
                    close(events[i].data.fd);
                }
                else if(recvRes > 0)
                {
                    send(events[i].data.fd, buf, recvRes, MSG_NOSIGNAL);
                }
            }
        }
    }

    return 0;
}