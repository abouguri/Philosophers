/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:47:48 by abouguri          #+#    #+#             */
/*   Updated: 2024/12/05 23:52:42 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define SUCCESS 0
# define ERROR_WRONG_INPUT 1
# define ERROR_MALLOC_FAILURE 2
# define ERROR_MUTEX_INIT 3
# define ERROR_THREAD_CREATION 4
# define ERROR_THREAD_JOIN 5
# define PHILO_TAKE_FORKS "has taken a fork"
# define PHILO_THINK "is thinking"
# define PHILO_SLEEP "is sleeping"
# define PHILO_EAT "is eating"
# define PHILO_DIED "died"
# define EAT_TIME_OFFSET 10

# define INT_MIN -2147483648
# define INT_MAX 2147483647

# define EAT_TIME_OFFSET 10

# define INT_MIN -2147483648
# define INT_MAX 2147483647

typedef enum e_philo_state
{
	EATING = 0,
	SLEEPING = 1,
	THINKING = 2,
	DEAD = 3,
	FULL = 4,
	IDLE = 5
}					t_state;

typedef struct s_philo
{
	int				id;
	int				num_meals_had;
	uint64_t		last_eat_time;
	struct s_data	*data;
	t_state			state;
	pthread_mutex_t	mutex_state;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	mutex_num_meals_had;
	pthread_mutex_t	mutex_last_eat_time;
}					t_philo;

// Shared Simulation Data
typedef struct s_data
{
	int				n_philos;
	int				nb_meals;
	int				nb_full_p;
	bool			keep_iterating;
	uint64_t		eat_time;
	uint64_t		die_time;
	uint64_t		sleep_time;
	uint64_t		start_time;
	pthread_mutex_t	mutex_eat_time;
	pthread_mutex_t	mutex_die_time;
	pthread_mutex_t	mutex_sleep_time;
	pthread_mutex_t	mutex_print;
	pthread_mutex_t	mutex_n_philos;
	pthread_mutex_t	mutex_keep_iter;
	pthread_mutex_t	mutex_start_time;
	pthread_t		monitor_all_alive;
	pthread_t		monitor_all_full;
	pthread_t		*philo_threads;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}					t_data;

int					initialize_simulation(int ac, char **av);
void				cleanup_simulation(t_data *data);
int					start_threads(t_data *data);
int					wait_for_threads(t_data *data);

int					is_input_numeric(int ac, char **av);
int					validate_input_constraints(int ac, char **av);
int					validate_arguments(int ac, char **av);
void				display_usage(void);
int					ft_atoi(const char *str);

int					malloc_data(t_data *data);
int					initialize_data(t_data *data, int ac, char **av);
int					initialize_philos(t_data *data);
int					initialize_forks(t_data *data);
void				assign_forks(t_data *data);

void				cleanup_philosophers(t_philo *philos, int count);
void				cleanup_forks(t_data *data, int count);

uint64_t			get_time(void);
void				ft_usleep(uint64_t sleep_time);
uint64_t			fetch_die_time(t_data *data);
uint64_t			fetch_sleep_time(t_data *data);
uint64_t			fetch_eat_time(t_data *data);
uint64_t			fetch_last_eat_time(t_philo *philo);
void				update_last_meal_time(t_philo *philo);

int					initialize_philosopher_mutexes(t_philo *philo);
void				set_keep_iterating(t_data *data, bool set_to);
t_state				fetch_philo_state(t_philo *philo);
void				update_philo_state(t_philo *philo, t_state state);
bool				is_simulation_running(t_data *data);
bool				philo_died(t_philo *philo);

int					take_forks(t_philo *philo);
int					take_left_fork(t_philo *philo);
int					take_right_fork(t_philo *philo);
void				unlock_left_fork(t_philo *philo);
void				unlock_right_fork(t_philo *philo);
void				unlock_forks(t_philo *philo);

int					solo_philo_case(t_philo *philo);
int					eat(t_philo *philo);
int					think(t_philo *philo);
int					philo_sleep(t_philo *philo);
void				sleep_for_eating(t_philo *philo);
void				update_num_meals_had(t_philo *philo);

void				*routine(void *philo_p);
void				*all_alive_routine(void *data_p);
void				*all_full_routine(void *data_p);
void				set_all_philos_dead(t_data *data);
bool				is_philo_done(t_data *data, t_philo *philo);
bool				has_meals_option(t_data *data);

void				print_msg(t_data *data, int id, char *msg);

int					get_philo_meals_had(t_philo *philo);
uint64_t			fetch_start_time(t_data *data);
int					philos_count(t_data *data);

#endif
