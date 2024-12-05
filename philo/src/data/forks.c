/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:37:15 by abouguri          #+#    #+#             */
/*   Updated: 2024/12/05 17:12:30 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void assign_forks(t_data *data)
{
    int i;
    t_philo *philos = data->philos;

    i = 0;
    philos[0].left_fork = &data->forks[0];
    philos[0].right_fork = &data->forks[data->n_philos - 1];

    while (++i < data->n_philos)
    {
        philos[i].left_fork = &data->forks[i];
        philos[i].right_fork = &data->forks[i - 1];
    }
}

void cleanup_forks(t_data *data, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        pthread_mutex_destroy(&data->forks[i]);
        i++;
    }
}

int	take_forks(t_philo *philo)
{
    if (philos_count(philo->data) == 1)
		return (solo_philo_case(philo));
    if (take_right_fork(philo) != 0)
    {
		unlock_right_fork(philo);
		return (1);
	}
    if (take_left_fork(philo) != 0)
	{
		unlock_right_fork(philo);
		return (1);
	}
	return (0);
}

void    unlock_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}
