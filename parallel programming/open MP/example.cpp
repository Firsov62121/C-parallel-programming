#include <iostream>
//link with -fopenmp
//https://www.openmp.org/wp-content/uploads/openmp-4.5.pdf
//export OMP_NUM_THREADS = 4;
int main()
{
    //omp_set_num_threads(4);
    #pragma omp parallel num_threads(2)//shared(A, b, c) private(e, f, g) 
    {
        std::cout << "Hello, world!" << std::endl;
    }
    #pragma omp parallel for 
    for(uint i = 0; i < (uint)1e7; ++i)
        i * i;
    //or
    #pragma omp parallel
    {
        #pragma parallel for
        for(uint i = 0; i < (uint)1e7; ++i)
            i * i;
    }
    int sum = 0;
    #pragma omp parallel for num_threads(4) reduction(+ : sum) 
    for(int i = 0; i < (uint)1e8; ++i)
    {
        sum += i;
    }
    std::cout << sum << std::endl;
    std::cout << "barrier:\n";
    #pragma omp parallel num_threads(3)
    {
        std::cout << 1 << std::endl;
        std::cout << 3 << std::endl;
        #pragma omp barrier //как в posix
        std::cout << 2 << std::endl;
    }
    std::cout << "single:\n";
    #pragma omp parallel num_threads(3)
    {
        std::cout << 1 << std::endl;
        #pragma omp single nowait
        {
            std::cout << 2 << std::endl;
        }//неявный барьер, если нет nowait
        #pragma omp master 
        //аналогично, но в главном потоке
        {
            std::cout << 4 << std::endl;
        }
        std::cout << 3 << std::endl;
    }
    std::cout << "oredered cycle\n";
    #pragma omp parallel for ordered num_threads(4)
    for(uint i = 0; i < 10; ++i)
    {
        std::cout << "cycle1!\n";
        #pragma omp ordered
        {
            std::cout << "cycle2!" << i << std::endl;
        }
    }
    std::cout << "critical section\n";
    int j = 0;
    #pragma omp parallel num_threads(3) shared(j)
    {
        #pragma omp critical
        {
            ++j;
            std::cout << "critical!" << j <<  std::endl;
            //выполняются отдельно друг от друга
        }
    }
    //atomic operations
    #pragma omp parallel
    {
        #pragma omp atomic
        ++j;
    }
    //sections
    #pragma omp sections //3 параллельных вычисления
    {
        #pragma omp section
        {
            //...
        }
        #pragma omp section
        {
            //...
        }
        #pragma omp section
        {
            //...
        }
    }
    //task
    #pragma omp task //<=> detach()
    {
        //...
    }
    #pragma omp taskwait //ждём завершения всех задач
    return 0;
}