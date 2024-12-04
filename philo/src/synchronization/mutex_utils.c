/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukia <rukia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:28:40 by rukia             #+#    #+#             */
/*   Updated: 2024/12/04 03:44:11 by rukia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int initialize_philosopher_mutexes(t_philo *philo)
{
    if (pthread_mutex_init(&philo->mutex_state, NULL) != 0)
        return (ERROR_MUTEX_INIT);
    if (pthread_mutex_init(&philo->mutex_num_meals_had, NULL) != 0)
    {
        pthread_mutex_destroy(&philo->mutex_state);
        return (ERROR_MUTEX_INIT);
    }
    if (pthread_mutex_init(&philo->mutex_last_eat_time, NULL) != 0)
    {
        pthread_mutex_destroy(&philo->mutex_state);
        pthread_mutex_destroy(&philo->mutex_num_meals_had);
        return (ERROR_MUTEX_INIT);
    }
    return (SUCCESS);
}

int	take_left_fork(t_philo *philo)
{
	if (philo_died(philo) || fetch_philo_state(philo) == DEAD)
		return (1);
	pthread_mutex_lock(philo->left_fork);
	print_msg(philo->data, philo->id, PHILO_TAKE_FORKS);
	return (0);
}

int	take_right_fork(t_philo *philo)
{
	if (philo_died(philo) || fetch_philo_state(philo) == DEAD)
		return (1);
	pthread_mutex_lock(philo->right_fork);
	print_msg(philo->data, philo->id, PHILO_TAKE_FORKS);
	return (0);
}

void unlock_left_fork(t_philo *philo)
{
    pthread_mutex_unlock(philo->left_fork);
}

void unlock_right_fork(t_philo *philo)
{
    pthread_mutex_unlock(philo->right_fork);
}
