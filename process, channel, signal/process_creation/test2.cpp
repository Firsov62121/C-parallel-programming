#include <signal.h>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char* argv[]) {
    if(fork())
    {
        std::ofstream f("./pid_parent");
        f << getpid();
        f.close();
        int s;
        wait(&s);
        while(1)
            sleep(1);
    }
    else
    {
        std::ofstream f("./pid_child");
        f << getpid();
        f.close();
        signal(SIGTERM, SIG_DFL);

        while(1)
            sleep(1);
    }

    return 0;
}


/*
Задача на зомби.

Создайте два процесса - родитель и потомок (сохраните их PIDы в /home/box/pid_parent и /home/box/pid_child).

Потомок должен убиваться по SIGTERM. При убийстве потомка не должно остаться процессов-зомби.
*/