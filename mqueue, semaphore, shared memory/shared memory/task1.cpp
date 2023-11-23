#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
/*
Общая память SysV.
Создайте общую память SysV (не менее 1 мегабайта) с ключем:
key_t key = ftok("/tmp/mem.temp", 1);
Каждый байт первого мегабайта общей памяти заполните числом 42.
*/

int main()
{
    key_t key;
    std::cout << (key = ftok("/tmp/mem.temp", 1)) << " ";
    int shmid = shmget(key, 2000000, IPC_CREAT | IPC_PRIVATE | 0666);
    if (shmid < 0) {
        std::cout << errno << std::endl;
        std::cout << "*** shmget error (client) ***\n";
        exit(1);
    }
    char *shm_ptr = (char *) shmat(shmid, nullptr, 0);
    if ((int) shm_ptr == -1)
    {
        std::cout << errno << std::endl;
        std::cout << "*** shmat error (client) ***\n";
        exit(1);
    }
    for(uint i = 0; i < 2000000; ++i)
    {
        shm_ptr[i] = 42;
    }
    shmdt(shm_ptr);
    return 0;
}