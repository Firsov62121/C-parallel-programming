#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>

/*
Создайте набор из 16 семафоров SysV с ключем:
key_t key = ftok("/tmp/sem.temp", 1);
Проинициализируйте их порядковыми номерами [0..15].
*/
#define MAXSETSIZE 16
int main()
{
    key_t key = ftok("./sem.temp", 1);
    std::cout << "key = " <<  key << std::endl;
    std::cout << "errno = " << errno << std::endl;
    int semid = semget(key, 16, IPC_CREAT | 0777);
    std::cout << "semid = " << semid << std::endl;
    union semnum{
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;
    arg.val = 0;
    std::cout << "semctl = " << semctl(semid, 0, IPC_RMID, arg) << std::endl;
    semid = semget(key, 16, IPC_CREAT | 0777);
    std::cout << "semid = " << semid << std::endl;
    // ushort array [MAXSETSIZE];
    // std::cout << semid << std::endl;
    // for(uint i = 0; i < 16; ++i)
    // {
    //     array[i] = 0;
    // }
    // std::cout << semctl(semid, 0, SETALL, array) << std::endl;
    struct sembuf sops[16];
    for (int i = 0; i < 16; i++) {
        sops[i].sem_num = i;
        sops[i].sem_op = i;
        sops[i].sem_flg = 0;
    }
    std::cout << "semop = " <<  semop(semid, sops, 16) << std::endl;
    return 0;

}