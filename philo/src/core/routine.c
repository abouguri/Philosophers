/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 20:42:36 by abouguri          #+#    #+#             */
/*   Updated: 2024/12/06 04:22:01 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	solo_philo_case(t_philo *philo)
{
	take_left_fork(philo);
	ft_usleep(fetch_die_time(philo->data));
	update_philo_state(philo, DEAD);
	return (1);
}

int eat(t_philo *philo)
{
    if (take_forks(philo) != 0)
        return (1);
    update_philo_state(philo, EATING);
    print_msg(philo->data, philo->id, PHILO_EAT);
    update_last_meal_time(philo);
    sleep_for_eating(philo);
    update_num_meals_had(philo);
    unlock_forks(philo);
    return (0);
}

int	think(t_philo *philo)
{
	update_philo_state(philo, THINKING);
	if (fetch_philo_state(philo) == DEAD)
		return (1);
	print_msg(philo->data, philo->id, PHILO_THINK);
	if (philo->data->n_philos == 3)
		ft_usleep(1);
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	update_philo_state(philo, SLEEPING);
	if (fetch_philo_state(philo) == DEAD)
		return (1);
	print_msg(philo->data, philo->id, PHILO_SLEEP);
	ft_usleep(fetch_sleep_time(philo->data));
	return (0);
}

void	*routine(void *philo_p)
{
	t_philo	*philo;

	philo = (t_philo *) philo_p;
	update_last_meal_time(philo);
	if (philo->id % 2 == 0)
		ft_usleep(philo->data->eat_time - EAT_TIME_OFFSET);
	while (fetch_philo_state(philo) != DEAD)
    {
        if (eat(philo) != 0 || fetch_philo_state(philo) == DEAD)
            break;
        if (philo_sleep(philo) != 0 || fetch_philo_state(philo) == DEAD)
            break;
        if (think(philo) != 0 || fetch_philo_state(philo) == DEAD)
            break;
    }
	return (NULL);
}