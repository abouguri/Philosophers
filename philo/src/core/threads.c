/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 20:34:05 by abouguri          #+#    #+#             */
/*   Updated: 2024/12/05 17:31:31 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	start_threads(t_data *data)
{
	int	i;
	int	n_philos;

	n_philos = philos_count(data);
	data->start_time = get_time();
	i = 0;
	while (i < n_philos)
	{
		if (pthread_create(&data->philo_threads[i], NULL, &routine,
				&data->philos[i]))
			return (ERROR_THREAD_CREATION);
		i++;
	}
	if (pthread_create(&data->monitor_all_alive, NULL, &all_alive_routine,
			data))
		return (ERROR_THREAD_CREATION);
	if (has_meals_option(data) && pthread_create(&data->monitor_all_full, NULL,
			&all_full_routine, data) != 0)
		return (ERROR_THREAD_CREATION);
	return (SUCCESS);
}

int	wait_for_threads(t_data *data)
{
	int	i;
	int	n_philos;

	n_philos = philos_count(data);
	i = -1;
	if (pthread_join(data->monitor_all_alive, NULL))
		return (ERROR_THREAD_JOIN);
	if (has_meals_option(data) == true && pthread_join(data->monitor_all_full,
			NULL))
		return (ERROR_THREAD_JOIN);
	while (++i < n_philos)
	{
		if (pthread_join(data->philo_threads[i], NULL))
			return (ERROR_THREAD_JOIN);
	}
	return (SUCCESS);
}

void	*all_alive_routine(void *data_p)
{
	int		i;
	int		n_philos;
	t_data	*data;
	t_philo	*philos;

	data = (t_data *)data_p;
	philos = data->philos;
	n_philos = philos_count(data);
	i = -1;
	while (is_simulation_running(data))
	{
		i = -1;
		while (++i < n_philos)
		{
			if (philo_died(&philos[i]) && is_simulation_running(data))
			{
				print_msg(data, philos[i].id, PHILO_DIED);
				set_keep_iterating(data, false);
				set_all_philos_dead(data);
				return (NULL);
			}
		}
		usleep(1000);
	}
	return (NULL);
}

void	*all_full_routine(void *data_p)
{
	t_data	*data;
	int		i;
	int		n_philos;

	data = (t_data *)data_p;
	i = -1;
	n_philos = philos_count(data);
	while (++i < n_philos && is_simulation_running(data))
	{
		usleep(1000);
		if (is_philo_done(data, &data->philos[i]) == false)
			i = -1;
	}
	if (is_simulation_running(data) == true)
	{
		set_keep_iterating(data, false);
		set_all_philos_dead(data);
	}
	return (NULL);
}
