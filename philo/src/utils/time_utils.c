/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukia <rukia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 20:53:36 by rukia             #+#    #+#             */
/*   Updated: 2024/12/04 04:10:27 by rukia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

uint64_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

uint64_t	fetch_die_time(t_data *data)
{
	uint64_t	die_time;

	pthread_mutex_lock(&data->mutex_die_time);
	die_time = data->die_time;
	pthread_mutex_unlock(&data->mutex_die_time);
	return (die_time);
}

uint64_t	fetch_sleep_time(t_data *data)
{
	uint64_t	sleep_time;

	pthread_mutex_lock(&data->mutex_sleep_time);
	sleep_time = data->sleep_time;
	pthread_mutex_unlock(&data->mutex_sleep_time);
	return (sleep_time);
}

uint64_t	fetch_eat_time(t_data *data)
{
	uint64_t	eat_time;

	pthread_mutex_lock(&data->mutex_eat_time);
	eat_time = data->eat_time;
	pthread_mutex_unlock(&data->mutex_eat_time);
	return (eat_time);
}

uint64_t	fetch_last_eat_time(t_philo *philo)
{
	uint64_t	last_eat_time;

	pthread_mutex_lock(&philo->mutex_last_eat_time);
	last_eat_time = philo->last_eat_time;
	pthread_mutex_unlock(&philo->mutex_last_eat_time);
	return (last_eat_time);
}

void ft_usleep(uint64_t sleep_time)
{
    uint64_t start;

    start = get_time();
    while ((get_time() - start) < sleep_time)
        usleep(500);
}

void update_last_meal_time(t_philo *philo)
{
    pthread_mutex_lock(&philo->mutex_last_eat_time);
    philo->last_eat_time = get_time(); // Assume get_time() returns the current timestamp
    pthread_mutex_unlock(&philo->mutex_last_eat_time);
}