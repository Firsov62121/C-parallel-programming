
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
// FreeBSD, Mac OS
//#include <sys/event.h> - есть на MacOS
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
    sockAddr.sin_addr.s_addr = 0;
    bind(masterSocket, (struct sockaddr *)(&sockAddr), sizeof(sockAddr));
    set_nonblock(masterSocket);

    listen(masterSocket, SOMAXCONN);

    int Kqueue = kqueue();
    struct kevent Kevent;
    bzero(&Kevent, sizeof(Kevent));
    EV_SET(&Kevent, masterSocket, EVFILT_READ, EU_ADD, 0, 0, 0);
    kevent(Kqueue, &Kevent, 1, nullptr, 0, nullptr);

    while(true)
    {
        bzero(&Kevent, sizeof(kevent));
        kevent(Kqueue, nullptr, 0, &Kevent, 1, nullptr);
        if(Kevent.filter = EVFILT_READ)
        {
            if(Kevent.ident == masterSocket)
            {
                int slaveSocket = accept(masterSocket, 0, 0);
                set_nonblock(slaveSocket);

                bzero(&Kevent, sizeof(Kevent));
                EV_SET(&Kevent, slaveSocket, EVFILT_READ, EV_READ, 0, 0, 0);
                kevent(Kqueue, &Kevent, 1, nullptr, 0, nullptr);
            }
            else
            {
                static char buf[1024];
                int recvRes = recv(Kevent.ident, buf, 1024, MSG_NOSIGNAL);
                if(recvRes <= 0)
                {
                    close(Kevent.ident);
                    printf("disconnected!\n");
                }
                else if(recvRes > 0)
                {
                    send(Kevent.ident, buf, recvRes, MSG_NOSIGNAL);
                }
            }
        }
    }
    return 0;
}