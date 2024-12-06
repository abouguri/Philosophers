/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 20:24:44 by abouguri          #+#    #+#             */
/*   Updated: 2024/12/05 17:30:21 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	initialize_simulation(int ac, char **av)
{
	t_data	data;

	if (initialize_data(&data, ac, av) != 0)
		return (ERROR_MALLOC_FAILURE);
	if (initialize_philos(&data) != 0 || initialize_forks(&data) != 0)
	{
		cleanup_simulation(&data);
		return (ERROR_MALLOC_FAILURE);
	}
	start_threads(&data);
	wait_for_threads(&data);
	cleanup_simulation(&data);
	return (SUCCESS);
}

int	initialize_philos(t_data *data)
{
	t_philo	*philos;
	int		i;

	i = 0;
	philos = data->philos;
	while (i < data->n_philos)
	{
		philos[i].data = data;
		philos[i].id = i + 1;
		philos[i].num_meals_had = 0;
		philos[i].state = IDLE;
		if (initialize_philosopher_mutexes(&philos[i]) != SUCCESS)
		{
			cleanup_philosophers(philos, i);
			return (ERROR_MUTEX_INIT);
		}
		update_last_meal_time(&philos[i]);
		i++;
	}
	return (SUCCESS);
}

int	initialize_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
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

void	cleanup_philosophers(t_philo *philos, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&philos[i].mutex_state);
		pthread_mutex_destroy(&philos[i].mutex_num_meals_had);
		pthread_mutex_destroy(&philos[i].mutex_last_eat_time);
		i++;
	}
}
