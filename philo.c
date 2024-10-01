/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:30:55 by abouguri          #+#    #+#             */
/*   Updated: 2024/08/28 20:40:27 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdio.h>
#include <stdlib.h>

void    init_simulation(t_simulation *simulation, t_params *params)
{
    simulation->params = params;
    simulation->simulation_ended = 0;
    simulation->philosophers = malloc(sizeof(t_philosopher) * params->num_philosophers);
    simulation->forks = malloc(sizeof(pthread_mutex_t) * params->num_philosophers);
    int i = 0;
    while(i < params->num_philosophers)
    {
        simulation->philosophers[i].id = i;
        simulation->philosophers[i].meals_eaten = 0;
        simulation->philosophers[i].params = params;
        simulation->philosophers[i].left_fork = &simulation->forks[i];
        simulation->philosophers[i].right_fork = &simulation->forks[(i + 1) % params->num_philosophers];
        pthread_mutex_init(&simulation->forks[i], NULL);
        i++;
    }
    pthread_mutex_init(&simulation->print_lock, NULL);
}

void    sleep_philosopher(t_philosopher *philosopher)
{
    usleep(philosopher->params->time_to_sleep * 1000);
}

void    eat(t_philosopher *philosopher)
{
    struct  timeval current_time;

    usleep(philosopher->params->time_to_eat * 1000);
    gettimeofday(&current_time, NULL);
    philosopher->last_meal_time = current_time;
    philosopher->meals_eaten++;
}

void    think(t_philosopher *philosopher)
{
    usleep(5 % 1000);
}
void *philosopher_routine(void *arg) {
    t_philosopher *philosopher = (t_philosopher *)arg;

    while (1) {
        // Philosopher is thinking
        print_status(philosopher, "is thinking");
        think(philosopher);

        // Philosopher tries to pick up the left fork
        pthread_mutex_lock(philosopher->left_fork);
        print_status(philosopher, "has taken a fork");

        // Philosopher tries to pick up the right fork
        pthread_mutex_lock(philosopher->right_fork);
        print_status(philosopher, "has taken a fork");

        // Philosopher is eating
        print_status(philosopher, "is eating");
        eat(philosopher);

        // Philosopher puts down the right fork
        pthread_mutex_unlock(philosopher->right_fork);

        // Philosopher puts down the left fork
        pthread_mutex_unlock(philosopher->left_fork);

        // Philosopher is sleeping
        print_status(philosopher, "is sleeping");
        sleep_philosopher(philosopher);

        // Check if philosopher has eaten enough times (optional stopping condition)
        if (philosopher->params->num_times_to_eat > 0 &&
            philosopher->meals_eaten >= philosopher->params->num_times_to_eat) {
            break;
        }
    }

    return NULL;
}

void *monitor_philosopher(t_simulation *arg) {
    // t_philosopher *philosopher = (t_philosopher *)arg;
    struct timeval current_time;
    long time_diff;
    long timestamp;

    while (1) {
        int i = 0;
        while (i++ < arg->params->num_philosophers)
        {

            gettimeofday(&current_time, NULL);
            time_diff = (current_time.tv_sec - arg->philosophers[i].last_meal_time.tv_sec) * 1000 +
                        (current_time.tv_usec - arg->philosophers[i].last_meal_time.tv_usec) / 1000;
            timestamp = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
            if (time_diff > arg->philosophers[i].params->time_to_die) {
                pthread_mutex_lock(&arg->philosophers[i].params->print_lock);
                printf("%ld %d died\n", timestamp, arg->philosophers[i].id + 1);
                pthread_mutex_unlock(&arg->philosophers[i].params->print_lock);
                return NULL;  // End the thread after a philosopher dies
            }
        }
        
        // usleep(1000);  // Sleep briefly to reduce CPU usage
    }
    return NULL;
}


void print_status(t_philosopher *philosopher, const char *status) {
    long timestamp;

    // Calculate the timestamp relative to the start of the simulation
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    timestamp = (current_time.tv_sec - philosopher->params->start_time.tv_sec) * 1000;
    timestamp += (current_time.tv_usec - philosopher->params->start_time.tv_usec) / 1000;

    // Print the status with a timestamp in a thread-safe manner
    pthread_mutex_lock(&philosopher->params->print_lock);
    printf("%ld %d %s\n", timestamp, philosopher->id + 1, status);
    pthread_mutex_unlock(&philosopher->params->print_lock);
}

int main(int ac, char **av)
{
    if (ac < 5 || ac > 6)
    {
        printf("Usage: ./philosophers number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
        return 1;
    }
    // pthread_t monitor_thread;
    t_params params;
    params.num_philosophers = atoi(av[1]);
    params.time_to_die = atoi(av[2]);
    params.time_to_eat = atoi(av[3]);
    params.time_to_sleep = atoi(av[4]);
    params.num_times_to_eat = (ac == 6) ? atoi(av[5]) : -1;

    t_simulation simulation;

    init_simulation(&simulation, &params);
    gettimeofday(&params.start_time, NULL);
    
    int i = 0;
    while(i < params.num_philosophers)
    {
        pthread_create(&simulation.philosophers[i].thread, NULL, philosopher_routine, &simulation.philosophers[i]);
        i++;
    }


    // pthread_create(&monitor_thread, NULL, monitor_philosopher, &simulation);

    
    i = 0;
    while(i < params.num_philosophers)
    {
        pthread_join(simulation.philosophers[i].thread, NULL);
        i++;
    }

    // pthread_join(monitor_thread, NULL);
    return (0);
}
