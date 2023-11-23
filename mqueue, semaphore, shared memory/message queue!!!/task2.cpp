#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <fstream>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <iostream>
//g++ ... -lrt
int main()
{
    // if(fork()){
    //     struct mq_attr attr;
    //     attr.mq_flags = 0;
    //     attr.mq_maxmsg = 10;
    //     attr.mq_msgsize = 256;
    //     attr.mq_curmsgs = 0;
    //     mqd_t md_server = mq_open("/test.mq", O_RDONLY | O_CREAT, 0644, &attr);
    //     char buf[256] = {0};
    //     while(0 >= (int)mq_receive(md_server, buf, 256, nullptr));
    //     std::ofstream f("message.txt");
    //     std::cout << "good:" << buf << std::endl;
    //     f << buf;
    //     f.close();
    //     mq_unlink("/test.mq");
    // }
    // else
    // {
    //     sleep(1);
    //     mqd_t md_server = mq_open("/test.mq", O_WRONLY);
    //     mq_send(md_server, "TEst1\n", 7, 0);
    // }
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 256;
    attr.mq_curmsgs = 0;
    mqd_t md_server = mq_open("/test.mq", O_RDONLY | O_CREAT, 0644, &attr);
    char buf[256] = {0};
    while(0 >= (int)mq_receive(md_server, buf, 256, nullptr));
    std::ofstream f("/home/box/message.txt");
    f << buf;
    f.close();
    mq_unlink("/test.mq");
    return 0;
}