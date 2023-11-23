
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <set>
#include <algorithm>
#include <cerrno>

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
    while(true)
    {
        fd_set Set;
        FD_ZERO(&Set);
        FD_SET(masterSocket, &Set);
        for(auto iter = slaveSockets.begin(); iter != slaveSockets.end(); ++iter)
        {
            FD_SET(*iter, &Set);
        }

        int Max = std::max(masterSocket, *std::max_element(slaveSockets.begin(), 
            slaveSockets.end()));
        select(Max + 1, &Set, nullptr, nullptr, nullptr);

        for(auto iter = slaveSockets.begin(); iter != slaveSockets.end(); ++iter)
        {
            if(FD_ISSET(*iter, &Set))
            {
                static char buf[1024];
                int recvSize = recv(*iter, buf, 1024, MSG_NOSIGNAL);
                if(recvSize == 0 && errno != EAGAIN) //проверка, что не прерывались  по сигналу
                {
                    shutdown(*iter, SHUT_RDWR);
                    close(*iter);
                    slaveSockets.erase(iter);
                }
                else if(recvSize != 0)
                {
                    send(*iter, buf, recvSize, MSG_NOSIGNAL);
                }
            }
        }
        if(FD_ISSET(masterSocket, &Set))
        {
            int slaveSocket = accept(masterSocket, 0, 0);
            set_nonblock(slaveSocket);
            slaveSockets.insert(slaveSocket);
        }
    }

    return 0;
}