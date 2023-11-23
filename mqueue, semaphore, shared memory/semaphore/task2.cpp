#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <iostream>
#include <fcntl.h> 
#include <sys/stat.h> 
// Создайте семафор POSIX с именем:
// /test.sem
// Проинициализируйте его числом 66.

int main()
{
    sem_unlink("/test.sem");
    sem_t *semid = sem_open("test.sem", O_CREAT, O_RDWR, 0);
    std::cout << "sem_init = " << sem_init(semid, 1, 6) << std::endl;
    int pos = 0;
    std::cout << "sem_getvalue = " << sem_getvalue(semid, &pos) << std::endl;
    std::cout << "pos = " << pos << std::endl;
    return 0;
}
//g++ test.cpp -o test -lpthread -std=c++11