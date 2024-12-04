/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukia <rukia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:23:56 by rukia             #+#    #+#             */
/*   Updated: 2024/12/04 02:39:06 by rukia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int malloc_data(t_data *data)
{
    data->philos = malloc(sizeof(t_philo) * data->n_philos);
    if (data->philos == NULL)
		return (ERROR_MALLOC_FAILURE);
    data->forks = malloc(sizeof(pthread_mutex_t) * data->n_philos);
    if (data->forks == NULL)
		return (free(data->philos), ERROR_MALLOC_FAILURE);
    data->philo_threads = malloc(sizeof(pthread_t) * data->n_philos);
    if (data->philo_threads == NULL)
		return (free(data->philos), free(data->forks), ERROR_MALLOC_FAILURE);
    return (0);
}

int initialize_data(t_data  *data,int ac,char **av)
{
    data->nb_full_p = 0;
    data->keep_iterating = true;
    data->n_philos = ft_atoi(av[1]);
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
    pthread_mutex_init(&data->mutex_n_philos, NULL);
    pthread_mutex_init(&data->mutex_keep_iter, NULL);
    pthread_mutex_init(&data->mutex_start_time, NULL);
    return (malloc_data(data));
}

void	cleanup_simulation(t_data *data)
{
	int	i;
	int	n_philos;

	n_philos = philos_count(data);
	i = -1;
	while (++i < n_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].mutex_state);
		pthread_mutex_destroy(&data->philos[i].mutex_num_meals_had);
		pthread_mutex_destroy(&data->philos[i].mutex_last_eat_time);
	}
	pthread_mutex_destroy(&data->mutex_die_time);
	pthread_mutex_destroy(&data->mutex_eat_time);
	pthread_mutex_destroy(&data->mutex_sleep_time);
	pthread_mutex_destroy(&data->mutex_n_philos);
	pthread_mutex_destroy(&data->mutex_print);
	pthread_mutex_destroy(&data->mutex_keep_iter);
	pthread_mutex_destroy(&data->mutex_start_time);
	free(data->philo_threads);
	free(data->philos);
	free(data->forks);
}

