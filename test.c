#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void    *routine(void *arg)
{
    int index = *(int *)arg;
    int sum = 0;
    for (int j = 0; j < 5; j++)
    {
        sum += primes[index + j];
    }
    *(int *)arg = sum;
    return(arg);
}

int main(int ac, char **av)
{
    pthread_t   th[10];
    for (int i = 0; i < 2; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i * 5;
        if (pthread_create(&th[i], NULL, &routine, a) != 0)
        {
            perror("Failed to create thread");
        }
    }
    int globalSum = 0;
    for (int i = 0; i < 2; i++)
    {
        int* r;
        if (pthread_join(th[i], (void **) &r) != 0)
        {
            perror("Failed to create thread");
        }
        globalSum += *r;
        free(r);
    }
    printf("Global sum: %d\n", globalSum);
    return (0);
}