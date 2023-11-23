#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>

//link with -lrt
/*
Создайте общую память POSIX с именем:

/test.shm
Размер сегмента должен быть 1 мегабайт. Заполните каждый байт числом 13.
*/
//ipcrm, ipcs
int main()
{
    int fd = shm_open("/test.shm", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        std::cout << "shm_open_error\n";
        exit(0);
    }
    if(ftruncate(fd, 2000000) == -1)
    {
        std::cout << "ftruncate_error\n";
        exit(0);
    }
    char *buf = (char *)mmap(nullptr, 2000000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(buf == MAP_FAILED)
    {
        std::cout << "mmap_error\n";
        exit(0);
    }
    for(uint i = 0; i < 2000000; ++i)
        buf[i] = 13;
    //munmap(buf, 2000000);
    pause();
    return 0;
}