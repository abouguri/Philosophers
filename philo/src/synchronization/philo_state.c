/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukia <rukia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:33:43 by rukia             #+#    #+#             */
/*   Updated: 2024/12/04 03:47:46 by rukia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_state fetch_philo_state(t_philo *philo)
{
    t_state state;

    pthread_mutex_lock(&philo->mutex_state);
    state = philo->state;
    pthread_mutex_unlock(&philo->mutex_state);

    return (state);
}

void	update_philo_state(t_philo *philo, t_state state)
{
	pthread_mutex_lock(&philo->mutex_state);
	if (philo->state != DEAD)
		philo->state = state;
	pthread_mutex_unlock(&philo->mutex_state);
}

bool	philo_died(t_philo *philo)
{
	bool		result;
	t_data		*data;

	data = philo->data;
	result = false;
	if (get_time() - fetch_last_eat_time(philo) > fetch_die_time(data) && fetch_philo_state(philo) != EATING)
	{
		update_philo_state(philo, DEAD);
		result = true;
	}
	return (result);
}

bool	is_philo_done(t_data *data, t_philo *philo)
{
	bool	result;

	result = false;
	if (get_philo_meals_had(philo) >= data->nb_meals)
		result = true;
	return (result); 
}

bool	has_meals_option(t_data *data)
{
	if (data->nb_meals > 0)
		return (true);
	return (false);
}
