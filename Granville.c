#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <pthread.h>

#define BILLION 1000000000L

#define A 2
#define B 600000

int step;

int set_size_S = 0;
int set_S[B - A];

pthread_rwlock_t set_S_rwlock = PTHREAD_RWLOCK_INITIALIZER;
pthread_barrier_t set_size_S_barrier;

//Calculation function
void *calculate_granville(void *ind)
{
    int start_num = *((int *)ind);
    int local_current_set_size_S = 0;
    int div_sum, n, i;
    int rem = B % step - start_num + step;

    pthread_rwlock_rdlock(&set_S_rwlock);
    local_current_set_size_S = set_size_S;
    pthread_rwlock_unlock(&set_S_rwlock);

    for (n = start_num + A; n < B; n += step)
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
                printf("Found %d\n", n);
            }

            pthread_rwlock_wrlock(&set_S_rwlock);
            set_S[set_size_S++] = n;
            pthread_rwlock_unlock(&set_S_rwlock);
        }

        //If it is at the last iteration it shouldn't wait
        //This is to avoid threads hanging for other threads that exited the loop already
        if (n + rem < B)
        {
            pthread_barrier_wait(&set_size_S_barrier);
        }

        pthread_rwlock_wrlock(&set_S_rwlock);
        local_current_set_size_S = set_size_S;
        pthread_rwlock_unlock(&set_S_rwlock);

        if (n + rem < B)
        {
            pthread_barrier_wait(&set_size_S_barrier);
        }
    }
}

int main(int argc, char *argv[])
{
    int i;
    int nr_threads;
    pthread_t *id_threads;

    struct timespec start, stop;
    uint64_t diff;
    double total_time;

    set_S[0] = 1;
    set_size_S = 1;

    //Get program argument
    if (argc != 2)
    {
        printf("Usage: program.exe N\n");
        printf("Where N is the number of threads to be generated\n");
        return 1;
    }

    //Get number of threads
    nr_threads = atoi(argv[1]);

    if (nr_threads <= 0)
    {
        printf("Please input a positive non null number!\n");
        return 2;
    }

    step = nr_threads;

    pthread_barrier_init(&set_size_S_barrier, NULL, nr_threads);
    id_threads = (pthread_t *)malloc(nr_threads * sizeof(pthread_t));

    //Get initial time
    if (clock_gettime(CLOCK_REALTIME, &start) == -1)
    {
        printf("Clock gettime\n");
        return 1;
    }

    //Create threads
    for (i = 0; i < nr_threads; i++)
    {
        int *st_num = malloc(sizeof(int));
        *st_num = i;
        pthread_create(&id_threads[i], NULL, calculate_granville, st_num);
    }

    //Join threads
    for (i = 0; i < nr_threads; i++)
    {
        pthread_join(id_threads[i], NULL);
    }

    if (clock_gettime(CLOCK_REALTIME, &stop) == -1)
    {
        perror("clock gettime");
        return 1;
    }

    /* Compute the execution time*/
    diff = BILLION * (stop.tv_sec - start.tv_sec) + stop.tv_nsec - start.tv_nsec;
    total_time = (double)diff / BILLION;

    printf("Time = %lf seconds\n", total_time);

    return 0;
}