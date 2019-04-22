#include<iostream>
#include <chrono>
#include <omp.h>

#define A 2
#define B 100

using namespace std;

int main(int argc, char *argv[])
{
    int step;

    int set_size_S = 0;
    int set_S[B - A];

    int start_num;
    int local_current_set_size_S = 0;
    int div_sum, n, i;
    int rem; // = B % step - start_num + step;

    int nr_threads;

    chrono::high_resolution_clock::time_point t1, t2;

    set_S[0] = 1;
    set_size_S = 1;

    //Get program argument
    if (argc != 2)
    {
        cout << "Usage: program.exe N" << endl;
        cout << "Where N is the number of threads to be generated" << endl;
        return 1;
    }

    //Get number of threads
    nr_threads = atoi(argv[1]);

    if (nr_threads <= 0)
    {
        cout << "Please input a positive non null number!" << endl;
        return 2;
    }

    step = nr_threads;

    //Get initial time
    t1 = chrono::high_resolution_clock::now();

    omp_set_num_threads(nr_threads);
#pragma omp parallel private(i, n, start_num, div_sum, local_current_set_size_S, rem) shared(set_S, set_size_S)
    {
        start_num = omp_get_thread_num();
        n = start_num + A;
        rem = B % step - start_num + step;

#pragma omp critical 
        {
            local_current_set_size_S = set_size_S;
        }

        while (n < B)
        {
            div_sum = 0;
            for (i = 0; i < local_current_set_size_S; i++)
            {
                if (n % set_S[i] == 0 && set_S[i] < n)
                {
                    div_sum += set_S[i];
                }
                if (div_sum > n || set_S[i] >= n)
                {
                    break;
                }
            }

            if (div_sum <= n)
            {
                if (div_sum == n)
                {
                    cout << "Found " << n << endl;
                }

#pragma omp critical 
                {
                    set_S[set_size_S++] = n;
                }
            }

            //If it is at the last iteration it shouldn't wait
            //This is to avoid threads hanging for other threads that exited the loop already
            if (n + rem < B)
            {
#pragma omp barrier
            }

#pragma omp critical 
            {
                local_current_set_size_S = set_size_S;
            }

            if (n + rem < B)
            {
#pragma omp barrier
            }

            n += step;
        }
    }

    /* Compute the execution time*/
    t2 = chrono::high_resolution_clock::now();
    chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);

    cout << endl << "Total time: " << time_span.count() << " seconds." << endl;

    return 0;
}