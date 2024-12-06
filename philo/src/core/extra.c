/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 20:50:02 by abouguri          #+#    #+#             */
/*   Updated: 2024/12/06 23:04:17 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	philos_count(t_data	*data)
{
	return (data->n_philos);
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
