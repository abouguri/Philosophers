/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 20:53:36 by abouguri          #+#    #+#             */
/*   Updated: 2024/12/05 17:42:58 by abouguri         ###   ########.fr       */
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
