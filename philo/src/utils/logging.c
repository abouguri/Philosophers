/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukia <rukia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:26:52 by rukia             #+#    #+#             */
/*   Updated: 2024/12/05 03:00:09 by rukia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_msg(t_data *data, int id, char *msg)
{
	uint64_t	time;

	time = get_time() - fetch_start_time(data);
	pthread_mutex_lock(&data->mutex_print);
	if (is_simulation_running(data))
		printf("%llu %d %s\n", time, id, msg);
	pthread_mutex_unlock(&data->mutex_print);
}

void	set_keep_iterating(t_data *data, bool set_to)
{
	pthread_mutex_lock(&data->mutex_keep_iter);
	data->keep_iterating = set_to;
	pthread_mutex_unlock(&data->mutex_keep_iter);
}

void	set_all_philos_dead(t_data *data)
{
	t_philo	*philos;
	int		i;
	int		n_philos;

	n_philos = philos_count(data);
	philos = data->philos;
	i = -1;
	while (++i < n_philos)
		update_philo_state(&philos[i], DEAD);
}

uint64_t	fetch_start_time(t_data *data)
{
	uint64_t	time;

	pthread_mutex_lock(&data->mutex_start_time);
	time = data->start_time;
	pthread_mutex_unlock(&data->mutex_start_time);
	return (time);
}
