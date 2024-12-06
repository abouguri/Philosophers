/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:43:03 by abouguri          #+#    #+#             */
/*   Updated: 2024/12/05 17:43:16 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

void	ft_usleep(uint64_t sleep_time)
{
	uint64_t	start;

	start = get_time();
	while ((get_time() - start) < sleep_time)
		usleep(500);
}

void	update_last_meal_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex_last_eat_time);
	philo->last_eat_time = get_time();
	pthread_mutex_unlock(&philo->mutex_last_eat_time);
}
