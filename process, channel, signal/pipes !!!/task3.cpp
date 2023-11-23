#include <stdio.h>
#include <iostream>
#include <sys/file.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

/*
Тренировочная задача.

Создайте пару сокетов, соединяющую два процесса (файл с исходниками должен быть один и находиться в /home/box/work.cpp). Процесс должен быть запущен.

[Не оценивается] Потренируйтесь передавать дескрипторы через эту пару сокетов.
*/

int main()
{
    int sockets[2], child;
    char buf[1024];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0) {
        perror("opening stream socket pair");
        exit(1);
    }
    if (fork()) {     /* This is the parent. */
        close(sockets[0]);
        uint i = 0;
        while(true)
        {
            int len = 0;
            if ((len = read(sockets[1], buf, 1024)) > 0 && ++i < 10)
                printf("-->%s\n", buf);
        }
        write(sockets[1], "test 1\n", 8);
        close(sockets[1]);

    } else {     /* This is the child. */
        close(sockets[1]);
        while(true)
            write(sockets[0], "test 2", 7);
        close(sockets[0]);
    }
    pause();
}