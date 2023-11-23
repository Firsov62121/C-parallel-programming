#include <pthread.h>
#include <unistd.h>
#include <fstream>

/*
Напишите многопоточную программу (POSIX Threads).

Должны быть как минимум:

Один поток, ожидающий события по условной переменной.
Один поток, ожидающий достижения барьера.
Текст программы должен находиться в файле /home/box/main.cpp
PID запущенного процесса должен быть сохранен в файле /home/box/main.pid
*/

pthread_cond_t condV;

void * funcCondV(void * val)
{
    pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_wait(&condV, &m);
    return val;
}

void * funcBarrier(void * val)
{
    pthread_barrier_t barrier;
    pthread_barrierattr_t attr;
    pthread_barrierattr_init(&attr);
    pthread_barrier_init(&barrier, &attr, 1);
    pthread_barrier_wait(&barrier);
    return val;
}

int main()
{
    std::ofstream f("/home/box/main.pid");
    f << getpid() << std::endl;
    f.close();
    pthread_t tid1, tid2;
    pthread_attr_t attr1, attr2;
    pthread_attr_init(&attr1);
    pthread_attr_init(&attr2);
    int arg1 = 5, arg2 = 1;
    pthread_create(&tid1, &attr1, funcCondV, &arg1);
    pthread_create(&tid2, &attr2, funcBarrier, &arg2);
    int * val1, val2; 
    pthread_join(tid1, (void**)&val1);
    pthread_join(tid2, (void**)&val2);
}