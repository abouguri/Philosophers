/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukia <rukia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:30:55 by abouguri          #+#    #+#             */
/*   Updated: 2024/12/03 19:57:03 by rukia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int is_input_numeric(int ac, char **av)
{
    int arg_index;
    int char_index;
    
    arg_index = 1;
    while(arg_index < ac)
    {
        char_index = 0;
        while(av[arg_index][char_index])
        {
            if (av[arg_index][char_index] < '0' || av[arg_index][char_index] > '9')
                return (ERROR_WRONG_INPUT);
            char_index++;
        }
        arg_index++;
    }
    return (SUCCESS);
}

int ft_atoi(const char *str)
{
    int count = 0;
    long result = 0;
    int sign = 1;

    while (str[count] == '\r' || str[count] == '\t' || str[count] == ' '
        || str[count] == '\f' || str[count] == '\v' || str[count] == '\n')
        count++;
    if (str[count] == '-')
    {
        sign = -1;
        count++;
    }
    else if (str[count] == '+')
        count++;
    if (!(str[count] >= '0' && str[count] <= '9'))
        return (0);
    while (str[count] >= '0' && str[count] <= '9')
    {
        result = result * 10 + (str[count++] - '0');
        if ((result * sign) > INT_MAX || (result * sign) < INT_MIN)
            return (0);
    }
    return ((int)(result * sign));
}

int validate_input_constraints(int ac, char **av)
{
    int arg_index;

    arg_index = 1;
    if (ac == 6 && ft_atoi(av[5]) <= 0)
        return (ERROR_WRONG_INPUT);
    if (ft_atoi(av[arg_index]) < 1 || ft_atoi(av[arg_index]) > 200)
        return (ERROR_WRONG_INPUT);
    while (++arg_index < 5)
    {
        if (ft_atoi(av[arg_index]) < 60)
            return (ERROR_WRONG_INPUT);
    }
    return (SUCCESS);
}

int validate_arguments(int ac,char  **av)
{
    if (ac < 5 || ac > 6)
		return (ERROR_WRONG_INPUT);
	if (is_input_numeric(ac, av) != 0)
		return (ERROR_WRONG_INPUT);
	if (validate_input_constraints(ac, av))
		return (ERROR_WRONG_INPUT);
	return (SUCCESS);
}

void    display_usage(void)
{
    printf("\n!!!!!!!!WRONG INPUT!!!!!!!!\n\n");
    printf("Usage:\n");
    printf("  ./philo n_philos time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
    printf("\nExample:\n");
    printf("  ./philo 4 800 200 200 5\n\n");
    printf("Parameters:\n");
    printf("  n_philos*:                        1-200\n");
    printf("  time_to_die* (ms):                 60+\n");
    printf("  time_to_eat* (ms):                 60+\n");
    printf("  time_to_sleep* (ms):               60+\n");
    printf("  nb_each_philo_must_eat (Optional): 0+\n");
}

int malloc_data(t_data *data)
{
    data->philos = malloc(sizeof(t_philo) * data->n_philos);
    if (data->philos == NULL)
		return (ERROR_MALLOC_FAILURE);
    data->forks = malloc(sizeof(pthread_mutex_t) * data->n_philos);
    if (data->forks == NULL)
		return (free(data->philos), ERROR_MALLOC_FAILURE);
    data->philo_threads = malloc(sizeof(pthread_t) * data->n_philos);
    if (data->philo_threads == NULL)
		return (free(data->philos), free(data->forks), ERROR_MALLOC_FAILURE);
    return (0);
}

int initialize_data(t_data  *data,int ac,char **av)
{
    data->nb_full_p = 0;
    data->keep_iterating = true;
    data->n_philos = ft_atoi(av[1]);
    data->die_time = (uint64_t)ft_atoi(av[2]);
    data->eat_time = (uint64_t)ft_atoi(av[3]);
    data->sleep_time = (uint64_t)ft_atoi(av[4]);
    data->nb_meals = -1;

    if (ac == 6)
        data->nb_meals = ft_atoi(av[5]);
    pthread_mutex_init(&data->mutex_eat_time, NULL);
    pthread_mutex_init(&data->mutex_sleep_time, NULL);
    pthread_mutex_init(&data->mutex_die_time, NULL);
    pthread_mutex_init(&data->mutex_print, NULL);
    pthread_mutex_init(&data->mutex_n_philos, NULL);
    pthread_mutex_init(&data->mutex_keep_iter, NULL);
    pthread_mutex_init(&data->mutex_start_time, NULL);
    return (malloc_data(data));
}

void    cleanup_philosophers(t_philo *philos, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        pthread_mutex_destroy(&philos[i].mutex_state);
        pthread_mutex_destroy(&philos[i].mutex_num_meals_had);
        pthread_mutex_destroy(&philos[i].mutex_last_eat_time);
        i++;
    }
}

uint64_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

void update_last_meal_time(t_philo *philo)
{
    pthread_mutex_lock(&philo->mutex_last_eat_time);
    philo->last_eat_time = get_time(); // Assume get_time() returns the current timestamp
    pthread_mutex_unlock(&philo->mutex_last_eat_time);
}


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

int initialize_philos(t_data *data)
{
    t_philo *philos;
    int i;

    i = 0;
    philos = data->philos;
    while (i < data->n_philos)
	{
        philos[i].data = data;           // Link to global simulation data
        philos[i].id = i + 1;           // Assign philosopher ID (1-based)
        philos[i].num_meals_had = 0;     // Initialize meals eaten count
        philos[i].state = IDLE;         // Set initial state to IDLE
        
		if (initialize_philosopher_mutexes(&philos[i]) != SUCCESS)
        {
            cleanup_philosophers(philos, i); // Free initialized mutexes
            return (ERROR_MUTEX_INIT);
        }
		update_last_meal_time(&philos[i]);
        i++;
	}
    return (SUCCESS);
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

int initialize_forks(t_data *data)
{
    int i;

    i = 0;
    // Initialize mutexes for forks
    while (i < data->n_philos)
    {
        if (pthread_mutex_init(&data->forks[i], NULL) != 0)
        {
            cleanup_forks(data, i);
            return (ERROR_MUTEX_INIT);
        }
        i++;
    }
    assign_forks(data);
    return (SUCCESS);
}

int	philos_count(t_data *data)
{
	int	n_philos;

	pthread_mutex_lock(&data->mutex_n_philos);
	n_philos = data->n_philos;
	pthread_mutex_unlock(&data->mutex_n_philos);
	return (n_philos);
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

bool	is_simulation_running(t_data *data)
{
	bool	keep_iterating;

	pthread_mutex_lock(&data->mutex_keep_iter);
	keep_iterating = data->keep_iterating;
	pthread_mutex_unlock(&data->mutex_keep_iter);
	return (keep_iterating);
}

uint64_t	fetch_start_time(t_data *data)
{
	uint64_t	time;

	pthread_mutex_lock(&data->mutex_start_time);
	time = data->start_time;
	pthread_mutex_unlock(&data->mutex_start_time);
	return (time);
}

void	print_msg(t_data *data, int id, char *msg)
{
	uint64_t	time;

	time = get_time() - fetch_start_time(data);
	pthread_mutex_lock(&data->mutex_print);
	if (is_simulation_running(data))
		printf("%llu %d %s\n", time, id, msg);
	pthread_mutex_unlock(&data->mutex_print);
}

int	take_left_fork(t_philo *philo)
{
	if (philo_died(philo) || fetch_philo_state(philo) == DEAD)
		return (1);
	pthread_mutex_lock(philo->left_fork);
	print_msg(philo->data, philo->id, PHILO_TAKE_FORKS);
	return (0);
}

void ft_usleep(uint64_t sleep_time)
{
    uint64_t start;

    start = get_time(); // Get the current time in milliseconds
    while ((get_time() - start) < sleep_time)
        usleep(500); // Sleep for 500 microseconds to prevent busy-waiting
}

int	solo_philo_case(t_philo *philo)
{
	take_left_fork(philo);
	ft_usleep(fetch_die_time(philo->data));
	update_philo_state(philo, DEAD);
	return (1);
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


void    unlock_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

int	take_forks(t_philo *philo)
{
    if (philos_count(philo->data) == 1)
		return (solo_philo_case(philo));
    if (take_right_fork(philo) != 0)
		return (1);
    if (take_left_fork(philo) != 0)
	{
		unlock_right_fork(philo);
		return (1);
	}
	return (0);
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

int	eat(t_philo *philo)
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

void	set_keep_iterating(t_data *data, bool set_to)
{
	pthread_mutex_lock(&data->mutex_keep_iter);
	data->keep_iterating = set_to;
	pthread_mutex_unlock(&data->mutex_keep_iter);
}

void	set_all_philos_dead(t_data *data)
{
	t_philo	*philos;
	int		i;
	int		n_philos;

	n_philos = philos_count(data);
	philos = data->philos;
	i = -1;
	while (++i < n_philos)
		update_philo_state(&philos[i], DEAD);
}

void	*all_alive_routine(void *data_p)
{
	int		i;
	int		n_philos;
	t_data	*data;
	t_philo	*philos;

	data = (t_data *)data_p;
	philos = data->philos;
	n_philos = philos_count(data);
	i = -1;
    while (is_simulation_running(data))
    {
        i = -1;
        while (++i < n_philos)
        {
            if (philo_died(&philos[i]) && is_simulation_running(data))
            {
                print_msg(data, philos[i].id, PHILO_DIED);
                set_keep_iterating(data, false);
                set_all_philos_dead(data);
                return (NULL);
            }
        }
        usleep(1000); // Monitor at 1ms intervals
    }
	return (NULL);
}

int	get_philo_meals_had(t_philo *philo)
{
	int	num_meals_had;

	pthread_mutex_lock(&philo->mutex_num_meals_had);
	num_meals_had = philo->num_meals_had;
	pthread_mutex_unlock(&philo->mutex_num_meals_had);
	return (num_meals_had);
}

bool	is_philo_done(t_data *data, t_philo *philo)
{
	bool	result;

	result = false;
	if (get_philo_meals_had(philo) >= data->nb_meals)
		result = true;
	return (result); 
}

void	*all_full_routine(void *data_p)
{
	t_data	*data;
	int		i;
	int		n_philos;

	data = (t_data *)data_p;
	i = -1;
	n_philos = philos_count(data);
	while (++i < n_philos && is_simulation_running(data))
	{
		usleep(1000);
		if (is_philo_done(data, &data->philos[i]) == false)
			i = -1;
	}
	if (is_simulation_running(data) == true)
	{
		set_keep_iterating(data, false);
		set_all_philos_dead(data);
	}
	return (NULL);
}

bool	has_meals_option(t_data *data)
{
	if (data->nb_meals > 0)
		return (true);
	return (false);
}

int start_threads(t_data *data)
{
    int i;
    int nb_of_philos;

    nb_of_philos = philos_count(data); // Get the number of philosophers
    data->start_time = get_time();
    i = 0;
    while (i < nb_of_philos)
    {
        if (pthread_create(&data->philo_threads[i], NULL, &routine, &data->philos[i]))
            return (1);
        i++;
    }
    if (pthread_create(&data->monitor_all_alive, NULL, &all_alive_routine, data))
        return (1);
    if (has_meals_option(data) && pthread_create(&data->monitor_all_full, NULL, &all_full_routine, data) != 0)
        return (1);
    return (0);
}

int	wait_for_threads(t_data *data)
{
	int	i;
	int	n_philos;

	n_philos = philos_count(data);
	i = -1;
	if (pthread_join(data->monitor_all_alive, NULL))
		return (1);
	if (has_meals_option(data) == true && pthread_join(data->monitor_all_full, NULL))
		return (1);
	while (++i < n_philos)
	{
		if (pthread_join(data->philo_threads[i], NULL))
			return (1);
	}
	return (0);
}

void	cleanup_simulation(t_data *data)
{
	int	i;
	int	n_philos;

	n_philos = philos_count(data);
	i = -1;
	while (++i < n_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].mutex_state);
		pthread_mutex_destroy(&data->philos[i].mutex_num_meals_had);
		pthread_mutex_destroy(&data->philos[i].mutex_last_eat_time);
	}
	pthread_mutex_destroy(&data->mutex_die_time);
	pthread_mutex_destroy(&data->mutex_eat_time);
	pthread_mutex_destroy(&data->mutex_sleep_time);
	pthread_mutex_destroy(&data->mutex_n_philos);
	pthread_mutex_destroy(&data->mutex_print);
	pthread_mutex_destroy(&data->mutex_keep_iter);
	pthread_mutex_destroy(&data->mutex_start_time);
	free(data->philo_threads);
	free(data->philos);
	free(data->forks);
}

int initialize_simulation(int ac, char **av)
{
    t_data data;
    
    if (initialize_data(&data, ac, av) != 0)
        return (ERROR_MALLOC_FAILURE);
    initialize_philos(&data);
    initialize_forks(&data);
    start_threads(&data);
    wait_for_threads(&data);
    cleanup_simulation(&data);
    return (SUCCESS);
}

int main(int ac, char **av)
{
    if (validate_arguments(ac, av) != 0)
	{
		display_usage();
		return (ERROR_WRONG_INPUT);
	}
	if (initialize_simulation(ac, av) != 0)
		return (ERROR_MALLOC_FAILURE);
    return (SUCCESS);
}
