#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
// lick with -pthread
/*
Напишите многопоточную программу (POSIX Threads).

Должен быть как минимум:

Один поток, ожидающий pthread_join.
Текст программы должен находиться в файле /home/box/main.cpp
PID запущенного процесса должен быть сохранен в файле /home/box/main.pid
*/

typedef struct pargs {
    int* val;
    int id;
} pargs;

void *func(pargs *val)
{
    for (int i = 0; i < 10; ++i)
    {
        int val2 = *(val->val);
        usleep(15);
        *(val->val) = val2 + 1;
        printf("val = %d, thread_id = %d\n", *(val->val), (val->id));
    }

    return (void *)val;
}

int main()
{
    // std::ofstream f("./main.pid");
    // f << getpid() << std::endl;
    // f.close();
    pthread_t thread_id;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int arg = 0;
    pargs args;
    args.val = &arg;
    args.id = 0;
    pargs args2;
    args2.val = &arg;
    args2.id = 1;
    pthread_create(&thread_id, &attr, (void *(*)(void *))func, &args);
    pthread_t thread_id2;
    pthread_attr_t attr2;
    pthread_attr_init(&attr2);
    pthread_create(&thread_id2, &attr2, (void *(*)(void *))func, &args2);
    int *val;
    pthread_join(thread_id, (void **)&val);
    pthread_join(thread_id2, (void **)&val);
    return 0;
}