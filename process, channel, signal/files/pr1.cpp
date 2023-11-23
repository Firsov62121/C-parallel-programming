#include <iostream>
#include <fcntl.h>
#include <sstream>
#include <sys/file.h>
#include <unistd.h>

int main()
{
    if(fork())
    {
        std::cout << "pr1" << getpid() << std::endl;
        int f = open("/tmp/file.txt", O_RDWR | O_CREAT, 0666);
        flock(f, LOCK_SH);
        pause();
        exit(0);
    }
    else
    {
        sleep(5);
        std::cout << "pr2" << getpid() << std::endl;
        int f = open("/tmp/file.txt", O_RDWR);
        flock(f, LOCK_EX);
        printf("Ok!");
        exit(0);
    }
    return 0;
}