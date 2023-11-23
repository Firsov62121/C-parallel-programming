#include <pthread.h>
#include <fstream>
#include <unistd.h>
/*
Напишите многопоточную программу (POSIX Threads).

Должны быть как минимум:

Один поток, ожидающий освобождения мьютекса.
Один поток, ожидающий освобождения спин-блокировки.
Два потока, ожидающих освобождения RW-блокировки (один должен ожидать чтения, другой - записи).
Текст программы должен находиться в файле /home/box/main.cpp
PID запущенного процесса должен быть сохранен в файле /home/box/main.pid
*/

pthread_mutex_t mx;
pthread_spinlock_t mspin;
pthread_rwlock_t mread;
pthread_rwlock_t mwrite;

void * funcMutex2(void *val)
{
    pthread_mutex_lock(&mx);
    while(true);
    pthread_mutex_unlock(&mx);
}

void * funcMutex(void * val)
{
    pthread_t thread_id;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int arg = 5;
    pthread_create(&thread_id, &attr, funcMutex2, &arg);
    pthread_mutex_lock(&mx);
    pthread_join(thread_id, (void**)&val);
    return val;
}

void * funcMSpin2(void *val)
{
    pthread_spin_lock(&mspin);
    while(true);
    pthread_spin_unlock(&mspin);
    return val;
}

void * funcMSpin(void * val)
{
    pthread_t thread_id;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int arg = 5;
    pthread_create(&thread_id, &attr, funcMSpin2, &arg);
    pthread_spin_lock(&mspin);
    pthread_join(thread_id, (void**)&val);
    return val;
}

void * funcRead2(void *val)
{
    pthread_rwlock_rdlock(&mread);
    while(true);
    pthread_rwlock_unlock(&mread);
    return val;
}

void * funcRead(void * val)
{
    pthread_t thread_id;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int arg = 5;
    pthread_create(&thread_id, &attr, funcRead2, &arg);
    pthread_rwlock_rdlock(&mread);
    pthread_join(thread_id, (void**)&val);
    return val;
}

void * funcWrite2(void *val)
{
    pthread_rwlock_wrlock(&mwrite);
    while(true);
    pthread_rwlock_unlock(&mwrite);
    return val;
}

void * funcWrite(void * val)
{
    pthread_t thread_id;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int arg = 5;
    pthread_create(&thread_id, &attr, funcMSpin2, &arg);
    pthread_rwlock_wrlock(&mwrite);
    pthread_join(thread_id, (void**)&val);
    return val;
}

int main()
{
    std::ofstream f("/home/box/main.pid");
    f << getpid() << std::endl;
    f.close();
    mx = PTHREAD_MUTEX_INITIALIZER;
    pthread_spin_init(&mspin, PTHREAD_PROCESS_PRIVATE);
    mread = PTHREAD_RWLOCK_INITIALIZER;
    mwrite = PTHREAD_RWLOCK_INITIALIZER;
    pthread_t thread_id1, thread_id2, thread_id3, thread_id4;
    pthread_attr_t attr1, attr2, attr3, attr4;
    pthread_attr_init(&attr1);
    pthread_attr_init(&attr2);
    pthread_attr_init(&attr3);
    pthread_attr_init(&attr4);
    int arg1 = 5, arg2 = 6, arg3 = 7, arg4 =1;
    pthread_create(&thread_id1, &attr1, funcMutex, &arg1);
    pthread_create(&thread_id2, &attr2, funcMSpin, &arg2);
    pthread_create(&thread_id3, &attr3, funcRead, &arg3);
    pthread_create(&thread_id4, &attr4, funcWrite, &arg4);
    int * val1, val2, val3, val4;
    pthread_join(thread_id1, (void**)&val1);
    pthread_join(thread_id2, (void**)&val2);
    pthread_join(thread_id3, (void**)&val3);
    pthread_join(thread_id4, (void**)&val4);
}