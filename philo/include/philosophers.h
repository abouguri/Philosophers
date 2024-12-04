#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <stdint.h>
# include <stdbool.h>

// Macros for Return Values
# define SUCCESS                 0
# define ERROR_WRONG_INPUT       1
# define ERROR_MALLOC_FAILURE    2
# define ERROR_MUTEX_INIT        3
# define ERROR_THREAD_CREATION   4
# define ERROR_THREAD_JOIN       5

// Philosopher States
# define PHILO_TAKE_FORKS        "has taken a fork"
# define PHILO_THINK             "is thinking"
# define PHILO_SLEEP             "is sleeping"
# define PHILO_EAT               "is eating"
# define PHILO_DIED              "died"

# define EAT_TIME_OFFSET         10

// Limits
# define INT_MIN                 -2147483648
# define INT_MAX                 2147483647

// Enumeration for Philosopher States
typedef enum e_philo_state
{
    EATING    = 0,   // Philosopher is eating
    SLEEPING  = 1,   // Philosopher is sleeping
    THINKING  = 2,   // Philosopher is thinking
    DEAD      = 3,   // Philosopher has died
    FULL      = 4,   // Philosopher is full
    IDLE      = 5    // Philosopher is idle
} t_state;

// Philosopher Structure
typedef struct s_philo
{
    int             id;                 // Philosopher ID
    int             num_meals_had;      // Meals eaten count
    uint64_t        last_eat_time;      // Last meal timestamp
    struct s_data  *data;               // Pointer to shared simulation data
    t_state         state;              // Current state of the philosopher
    pthread_mutex_t mutex_state;        // Mutex for state protection
    pthread_mutex_t *left_fork;         // Pointer to the left fork
    pthread_mutex_t *right_fork;        // Pointer to the right fork
    pthread_mutex_t mutex_num_meals_had; // Mutex for meal count
    pthread_mutex_t mutex_last_eat_time; // Mutex for last meal timestamp
} t_philo;

// Shared Simulation Data
typedef struct s_data
{
    int             n_philos;           // Number of philosophers
    int             nb_meals;           // Meals required to finish
    int             nb_full_p;          // Count of full philosophers
    bool            keep_iterating;     // Simulation running flag
    uint64_t        eat_time;           // Time to eat
    uint64_t        die_time;           // Time to die without eating
    uint64_t        sleep_time;         // Time to sleep
    uint64_t        start_time;         // Simulation start time
    pthread_mutex_t mutex_eat_time;     // Mutex for eat_time
    pthread_mutex_t mutex_die_time;     // Mutex for die_time
    pthread_mutex_t mutex_sleep_time;   // Mutex for sleep_time
    pthread_mutex_t mutex_print;        // Mutex for atomic printing
    pthread_mutex_t mutex_n_philos;     // Mutex for philosopher count
    pthread_mutex_t mutex_keep_iter;    // Mutex for simulation flag
    pthread_mutex_t mutex_start_time;   // Mutex for start_time
    pthread_t       monitor_all_alive;  // Thread monitoring life states
    pthread_t       monitor_all_full;   // Thread monitoring meals
    pthread_t      *philo_threads;      // Array of philosopher threads
    pthread_mutex_t *forks;             // Array of fork mutexes
    t_philo        *philos;             // Array of philosopher data
} t_data;

// Function Prototypes

// Core
int     initialize_simulation(int ac, char **av);
void    cleanup_simulation(t_data *data);
int     start_threads(t_data *data);
int     wait_for_threads(t_data *data);

// Input Validation
int     is_input_numeric(int ac, char **av);
int     validate_input_constraints(int ac, char **av);
int     validate_arguments(int ac, char **av);
void    display_usage(void);
int     ft_atoi(const char *str);

// Data Initialization
int     malloc_data(t_data *data);
int     initialize_data(t_data *data, int ac, char **av);
int     initialize_philos(t_data *data);
int     initialize_forks(t_data *data);
void    assign_forks(t_data *data);

// Cleanup
void    cleanup_philosophers(t_philo *philos, int count);
void    cleanup_forks(t_data *data, int count);

// Time Utilities
uint64_t get_time(void);
void    ft_usleep(uint64_t sleep_time);
uint64_t fetch_die_time(t_data *data);
uint64_t fetch_sleep_time(t_data *data);
uint64_t fetch_eat_time(t_data *data);
uint64_t fetch_last_eat_time(t_philo *philo);
void    update_last_meal_time(t_philo *philo);

// Synchronization
int     initialize_philosopher_mutexes(t_philo *philo);
void    set_keep_iterating(t_data *data, bool set_to);
t_state fetch_philo_state(t_philo *philo);
void    update_philo_state(t_philo *philo, t_state state);
bool    is_simulation_running(t_data *data);
bool    philo_died(t_philo *philo);

// Fork Handling
int     take_forks(t_philo *philo);
int     take_left_fork(t_philo *philo);
int     take_right_fork(t_philo *philo);
void    unlock_left_fork(t_philo *philo);
void    unlock_right_fork(t_philo *philo);
void    unlock_forks(t_philo *philo);

// Philosopher Actions
int     solo_philo_case(t_philo *philo);
int     eat(t_philo *philo);
int     think(t_philo *philo);
int     philo_sleep(t_philo *philo);
void    sleep_for_eating(t_philo *philo);
void    update_num_meals_had(t_philo *philo);

// Monitoring
void    *routine(void *philo_p);
void    *all_alive_routine(void *data_p);
void    *all_full_routine(void *data_p);
void    set_all_philos_dead(t_data *data);
bool    is_philo_done(t_data *data, t_philo *philo);
bool    has_meals_option(t_data *data);

// Logging
void    print_msg(t_data *data, int id, char *msg);

// Miscellaneous
int     get_philo_meals_had(t_philo *philo);
uint64_t fetch_start_time(t_data *data);
int     philos_count(t_data *data);

#endif
