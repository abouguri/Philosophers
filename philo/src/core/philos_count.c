/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_count.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukia <rukia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 20:50:02 by rukia             #+#    #+#             */
/*   Updated: 2024/12/04 05:24:00 by rukia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


int	philos_count(t_data *data)
{
	int	n_philos;

	pthread_mutex_lock(&data->mutex_n_philos);
	n_philos = data->n_philos;
	pthread_mutex_unlock(&data->mutex_n_philos);
	return (n_philos);
}

void	sleep_for_eating(t_philo *philo)
{
	ft_usleep(fetch_eat_time(philo->data));
}

void	update_num_meals_had(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex_num_meals_had);
	philo->num_meals_had++;
	pthread_mutex_unlock(&philo->mutex_num_meals_had);
}
