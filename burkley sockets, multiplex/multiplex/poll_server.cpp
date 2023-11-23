
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <set>
#include <algorithm>
#include <cerrno>
#include <poll.h>

#define POLL_SIZE 2048

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
    std::set<int> slaveSockets;
    
    struct sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(12345);
    sockAddr.sin_addr.s_addr = htons(INADDR_ANY);
    bind(masterSocket, (struct sockaddr *)(&sockAddr), sizeof(sockAddr));
    
    set_nonblock(masterSocket);

    listen(masterSocket, SOMAXCONN);

    struct pollfd Set[POLL_SIZE];
    Set[0].fd = masterSocket;
    Set[0].events = POLLIN;

    while(true)
    {
        uint index = 1;
        for(auto iter = slaveSockets.begin(); iter != slaveSockets.end(); ++iter)
        {
            Set[index].fd = *iter;
            Set[index].events = POLLIN;
            ++index;
        }
        uint setSize = 1 + slaveSockets.size();
        poll(Set, setSize, -1); //-1 - no timeout

        for(uint i = 0; i < setSize; ++i)
        {
            if(Set[i].revents & POLLIN)
            {
                if(i)
                {
                    static char buf[1024];
                    int recvSize = recv(Set[i].fd, buf, 1024, MSG_NOSIGNAL);
                    if(recvSize == 0 && errno != EAGAIN)
                    {
                        shutdown(Set[i].fd, SHUT_RDWR);
                        close(Set[i].fd);
                        slaveSockets.erase(Set[i].fd);
                    }
                    else if(recvSize != 0)
                    {
                        send(Set[i].fd, buf, recvSize, MSG_NOSIGNAL);
                    }
                }
                else
                {
                    int slaveSocket = accept(masterSocket, 0, 0);
                    set_nonblock(slaveSocket);
                    slaveSockets.insert(slaveSocket);
                }
            }
        }
    }

    return 0;
}