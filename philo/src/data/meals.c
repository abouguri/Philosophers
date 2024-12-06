/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meals.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:17:22 by abouguri          #+#    #+#             */
/*   Updated: 2024/12/05 22:03:05 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_philo_meals_had(t_philo *philo)
{
	int	num_meals_had;

	pthread_mutex_lock(&philo->mutex_num_meals_had);
	num_meals_had = philo->num_meals_had;
	pthread_mutex_unlock(&philo->mutex_num_meals_had);
	return (num_meals_had);
}

bool	is_simulation_running(t_data *data)
{
	bool	keep_iterating;

	pthread_mutex_lock(&data->mutex_keep_iter);
	keep_iterating = data->keep_iterating;
	pthread_mutex_unlock(&data->mutex_keep_iter);
	return (keep_iterating);
}
