/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukia <rukia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:30:55 by abouguri          #+#    #+#             */
/*   Updated: 2024/12/01 20:32:05 by rukia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int is_input_numeric(int ac, char **av)
{
    int arg_index;
    int char_index;
    
    arg_index = 1;
    while(arg_index < ac)
    {
        char_index = 0;
        while(av[arg_index][char_index])
        {
            if (av[arg_index][char_index] < '0' || av[arg_index][char_index] > '9')
                return (ERROR_WRONG_INPUT);
            char_index++;
        }
        arg_index++;
    }
    return (SUCCESS);
}

int ft_atoi(const char *str)
{
    int count = 0;
    long result = 0;
    int sign = 1;

    while (str[count] == '\r' || str[count] == '\t' || str[count] == ' '
        || str[count] == '\f' || str[count] == '\v' || str[count] == '\n')
        count++;
    if (str[count] == '-')
    {
        sign = -1;
        count++;
    }
    else if (str[count] == '+')
        count++;
    if (!(str[count] >= '0' && str[count] <= '9'))
        return (0);
    while (str[count] >= '0' && str[count] <= '9')
    {
        result = result * 10 + (str[count++] - '0');
        if ((result * sign) > INT_MAX || (result * sign) < INT_MIN)
            return (0);
    }
    return ((int)(result * sign));
}

int validate_input_constraints(int ac, char **av)
{
    int arg_index;

    arg_index = 1;
    if (ac == 6 && ft_atoi(av[5]) <= 0)
        return (ERROR_WRONG_INPUT);
    if (ft_atoi(av[arg_index]) < 1 || ft_atoi(av[arg_index]) > 200)
        return (ERROR_WRONG_INPUT);
    while (++arg_index < 5)
    {
        if (ft_atoi(av[arg_index]) < 60)
            return (ERROR_WRONG_INPUT);
    }
    return (SUCCESS);
}

int validate_arguments(int ac,char  **av)
{
    if (ac < 5 || ac > 6)
		return (ERROR_WRONG_INPUT);
	if (is_input_numeric(ac, av) != 0)
		return (ERROR_WRONG_INPUT);
	if (validate_input_constraints(ac, av))
		return (ERROR_WRONG_INPUT);
	return (SUCCESS);
}

void    display_usage(void)
{
    printf("\n!!!!!!!!WRONG INPUT!!!!!!!!\n\n");
    printf("Usage:\n");
    printf("  ./philo nb_philos time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
    printf("\nExample:\n");
    printf("  ./philo 4 800 200 200 5\n\n");
    printf("Parameters:\n");
    printf("  nb_philos*:                        1-200\n");
    printf("  time_to_die* (ms):                 60+\n");
    printf("  time_to_eat* (ms):                 60+\n");
    printf("  time_to_sleep* (ms):               60+\n");
    printf("  nb_each_philo_must_eat (Optional): 0+\n");
}

int malloc_data(t_data *data)
{
    data->philos = malloc(sizeof(t_philo) * data->nb_philos);
    if (data->philos == NULL)
		return (ERROR_MALLOC_FAILURE);
    data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
    if (data->forks == NULL)
		return (free(data->philos), ERROR_MALLOC_FAILURE);
    data->philo_threads = malloc(sizeof(pthread_t) * data->nb_philos);
    if (data->philo_threads == NULL)
		return (free(data->philos), free(data->forks), ERROR_MALLOC_FAILURE);
    return (0);
}

int initialize_data(t_data  *data,int ac,char **av)
{
    data->nb_full_p = 0;
    data->keep_iterating = true;
    data->nb_philos = ft_atoi(av[1]);
    data->die_time = (uint64_t)ft_atoi(av[2]);
    data->eat_time = (uint64_t)ft_atoi(av[3]);
    data->sleep_time = (uint64_t)ft_atoi(av[4]);
    data->nb_meals = -1;

    if (ac == 6)
        data->nb_meals = ft_atoi(av[5]);
    pthread_mutex_init(&data->mutex_eat_time, NULL);
    pthread_mutex_init(&data->mutex_sleep_time, NULL);
    pthread_mutex_init(&data->mutex_die_time, NULL);
    pthread_mutex_init(&data->mutex_print, NULL);
    pthread_mutex_init(&data->mutex_nb_philos, NULL);
    pthread_mutex_init(&data->mutex_keep_iter, NULL);
    pthread_mutex_init(&data->mutex_start_time, NULL);
    return (malloc_data(data));
}

void    cleanup_philosophers(t_philo *philos, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        pthread_mutex_destroy(&philos[i].mutex_state);
        pthread_mutex_destroy(&philos[i].mutex_nb_meals_had);
        pthread_mutex_destroy(&philos[i].mutex_last_eat_time);
        i++;
    }
}

uint64_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

void update_last_meal_time(t_philo *philo)
{
    pthread_mutex_lock(&philo->mutex_last_eat_time);
    philo->last_eat_time = get_time(); // Assume get_time() returns the current timestamp
    pthread_mutex_unlock(&philo->mutex_last_eat_time);
}


int initialize_philosopher_mutexes(t_philo *philo)
{
    if (pthread_mutex_init(&philo->mutex_state, NULL) != 0)
        return (ERROR_MUTEX_INIT);
    if (pthread_mutex_init(&philo->mutex_nb_meals_had, NULL) != 0)
    {
        pthread_mutex_destroy(&philo->mutex_state);
        return (ERROR_MUTEX_INIT);
    }
    if (pthread_mutex_init(&philo->mutex_last_eat_time, NULL) != 0)
    {
        pthread_mutex_destroy(&philo->mutex_state);
        pthread_mutex_destroy(&philo->mutex_nb_meals_had);
        return (ERROR_MUTEX_INIT);
    }
    return (SUCCESS);
}

int initialize_philos(t_data *data)
{
    t_philo *philos;
    int i;

    i = 0;
    philos = data->philos;
    while (i < data->nb_philos)
	{
        philos[i].data = data;           // Link to global simulation data
        philos[i].id = i + 1;           // Assign philosopher ID (1-based)
        philos[i].nb_meals_had = 0;     // Initialize meals eaten count
        philos[i].state = IDLE;         // Set initial state to IDLE
        
		if (initialize_philosopher_mutexes(&philos[i]) != SUCCESS)
        {
            cleanup_philosophers(philos, i); // Free initialized mutexes
            return (ERROR_MUTEX_INIT);
        }
		update_last_meal_time(&philos[i]);
        i++;
	}
    return (SUCCESS);
}

void cleanup_forks(t_data *data, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        pthread_mutex_destroy(&data->forks[i]);
        i++;
    }
}

void assign_forks(t_data *data)
{
    int i;
    t_philo *philos = data->philos;

    i = 0;
    philos[0].left_fork = &data->forks[0];
    philos[0].right_fork = &data->forks[data->nb_philos - 1];

    while (++i < data->nb_philos)
    {
        philos[i].left_fork = &data->forks[i];
        philos[i].right_fork = &data->forks[i - 1];
    }
}

int initialize_forks(t_data *data)
{
    int i;

    i = 0;
    // Initialize mutexes for forks
    while (i < data->nb_philos)
    {
        if (pthread_mutex_init(&data->forks[i], NULL) != 0)
        {
            cleanup_forks(data, i);
            return (ERROR_MUTEX_INIT);
        }
        i++;
    }
    assign_forks(data);
    return (SUCCESS);
}

int	philos_count(t_data *data)
{
	int	nb_philos;

	pthread_mutex_lock(&data->mutex_nb_philos);
	nb_philos = data->nb_philos;
	pthread_mutex_unlock(&data->mutex_nb_philos);
	return (nb_philos);
}

int start_threads(t_data *data)
{
    int i;
    int nb_of_philos;

    nb_of_philos = get_nb_philos(data); // Get the number of philosophers
    data->start_time = get_time();
    i = 0;
    
}

int initialize_simulation(int ac, char **av)
{
    t_data data;
    
    if (initialize_data(&data, ac, av) != 0)
        return (ERROR_MALLOC_FAILURE);
    initialize_philos(&data);
    initialize_forks(&data);
    start_threads(&data);
    // wait_for_threads(&data);
    // cleanup_simulation(&data);
    return (SUCCESS);
}

int main(int ac, char **av)
{
    if (validate_arguments(ac, av) != 0)
	{
		display_usage();
		return (ERROR_WRONG_INPUT);
	}
	if (initialize_simulation(ac, av) != 0)
		return (ERROR_MALLOC_FAILURE);
    return (SUCCESS);
}
