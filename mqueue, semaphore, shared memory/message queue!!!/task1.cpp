#include <iostream>
#include <fstream>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
struct message {
    long mtype;
    char mtext[80];
};
//ipcs, ipcrm
int main (int argc, char **argv)
{
    key_t key = ftok("./tmp/msg.temp", 1);   
    std::cout << key << std::endl;                                    
    int qid = msgget(key, 0777 | IPC_CREAT); 
    std::cout << qid << std::endl;
    if(fork())
    {
        while(true)
        {
            struct message msg;
            msgrcv (qid, &msg, sizeof (msg.mtext), 1, 0);
            if(strlen(msg.mtext) > 0)
            {
                std::ofstream f("./message.txt");
                std::cout << msg.mtext;
                f << msg.mtext;
                f.close();
                printf ("Server: message received.\n");
                std::cout << msg.mtype << " " <<  msg.mtext << std::endl;
                break;
            }
            else
            {
                //std::cout << "bad len\n";
            }
        }
        msgctl(qid, IPC_RMID, NULL);
    }
    else
    {
        message msg{1, "Hello frinend!\n"};
        msgsnd(qid, &msg, sizeof(msg), 0);
        std::cout << "sended\n";
    }
    // if(fork())
    // {
    //     key_t key = ftok("./tmp/msg.temp", 1);                                       
    //     int id = msgget(key, 0666);   
    //     struct message msg{1, {0}};
    //     while(true)
    //     {
    //         msgrcv (id, (struct msgbuf *)&msg, sizeof (struct message), 0, 0);
    //         if(strlen(msg.mtext) > 0)
    //             break;
    //     }
    //     std::ofstream f("message.txt");
    //     std::cout << msg.mtext;
    //     f << msg.mtext;
    //     f.close();
    // }
    // else
    // {
    //     key_t key = ftok("./tmp/msg.temp", 1);                                       
    //     int id = msgget(key, 0666);  
    //     struct message msg{1, "Hello!"};
    //     msgsnd (id, (void *) &msg, sizeof(msg), IPC_NOWAIT);
    //     exit(0);
    // }
    return 0;
}