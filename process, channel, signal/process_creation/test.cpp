#include <signal.h>
#include <fstream>
#include <unistd.h>


int main(int argc, char* argv[]) {
  std::ofstream f("/home/box/pid");
  f << getpid();
  f.close();

  signal(SIGTERM, SIG_IGN);
  signal(SIGINT, SIG_IGN);

  while (true)
    sleep(1);

  return 0;
}

//Создайте процесс (сохраните его PID в /home/box/pid), устойчивый к сигналам SIGTERM и SIGINT (неубиваемый).