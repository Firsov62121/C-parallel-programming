/*
Задача на именованные каналы.

Напишите программу, которая создает два именованных канала - /home/box/in.fifo и /home/box/out.fifo

Пусть программа читает in.fifo и все прочитанное записывает в out.fifo.
*/
//https://stackoverflow.com/questions/8507810/why-does-my-program-hang-when-opening-a-mkfifo-ed-pipe
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>


int main()
{
    if(fork())
    {
        mkfifo("./in.fifo", 0666);
        mkfifo("./out.fifo", 0666);
        
        int fd_read = open("/home/box/in.fifo", O_RDONLY|O_NONBLOCK, 0666);
        while(true)
        {
            char buf[1024];
            
            size_t len = read(fd_read, buf, 1024);
            if(len > 0)
            {
                int fd_write = open("./out.fifo", O_WRONLY | O_NONBLOCK, 0666);
                std::cout << buf;
                write(fd_write, buf, len);
                close(fd_write);
            }
        }
    }
    else
    {
        mkfifo("./in.fifo", 0666);
        while(true)
        {
            std::string s;
            std::cin >> s;
            int fd_write = open("./in.fifo", O_WRONLY|O_CREAT|O_TRUNC | O_NONBLOCK, 0666);
            write(fd_write, s.c_str(), s.length());
            close(fd_write);
        }
    }
    return 0;
}