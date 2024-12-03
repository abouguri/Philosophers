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

# define SUCCESS 0
# define ERROR_WRONG_INPUT 1
# define ERROR_MALLOC_FAILURE 2
# define ERROR_MUTEX_INIT 3

# define PHILO_TAKE_FORKS "has taken a fork"
# define PHILO_THINK "is thinking"
# define PHILO_SLEEP "is sleeping"
# define PHILO_EAT "is eating"
# define PHILO_DIED "died"

#define EAT_TIME_OFFSET 10

# define INT_MIN -2147483648
# define INT_MAX 2147483647

typedef enum e_philo_state
{
    EATING = 0,   // Philosopher is eating
    SLEEPING = 1, // Philosopher is sleeping
    THINKING = 2, // Philosopher is thinking
    DEAD = 3,     // Philosopher has died
    FULL = 4,     // Philosopher is full (has eaten required meals)
    IDLE = 5      // Philosopher is idle (e.g., before starting or waiting)
} t_state;

typedef struct s_philo
{
    // Philosopher-specific data
    int             id;                 // Philosopher ID
    int             num_meals_had;       // Number of meals eaten
    uint64_t        last_eat_time;      // Timestamp of the last meal (in ms)

    // Pointer to shared simulation data
    struct s_data   *data;              // Pointer to global simulation data

    // State management
    t_state         state;              // Current state (e.g., THINKING, EATING, SLEEPING)
    pthread_mutex_t mutex_state;        // Protects the philosopher's state

    // Forks
    pthread_mutex_t *left_fork;         // Pointer to the left fork mutex
    pthread_mutex_t *right_fork;        // Pointer to the right fork mutex

    // Synchronization
    pthread_mutex_t mutex_num_meals_had; // Protects num_meals_had
    pthread_mutex_t mutex_last_eat_time; // Protects last_eat_time
} t_philo;


typedef struct s_data
{
    // Simulation parameters
    int             n_philos;         // Number of philosophers
    int             nb_meals;          // Number of meals each philosopher must eat
    int             nb_full_p;         // Number of philosophers who have finished eating
    bool            keep_iterating;    // Flag to keep the simulation running

    // Timing parameters (in milliseconds)
    uint64_t        eat_time;          // Time it takes for a philosopher to eat
    uint64_t        die_time;          // Time before a philosopher dies without eating
    uint64_t        sleep_time;        // Time a philosopher spends sleeping
    uint64_t        start_time;        // Simulation start time

    // Synchronization primitives (mutexes)
    pthread_mutex_t mutex_eat_time;    // Protects eat_time
    pthread_mutex_t mutex_die_time;    // Protects die_time
    pthread_mutex_t mutex_sleep_time;  // Protects sleep_time
    pthread_mutex_t mutex_print;       // Ensures atomic printing
    pthread_mutex_t mutex_n_philos;   // Protects n_philos
    pthread_mutex_t mutex_keep_iter;   // Protects keep_iterating flag
    pthread_mutex_t mutex_start_time;  // Protects start_time

    // Threads
    pthread_t       monitor_all_alive; // Thread monitoring philosophers' survival
    pthread_t       monitor_all_full;  // Thread monitoring philosophers' meal completion
    pthread_t       *philo_threads;    // Array of philosopher threads

    // Philosopher-related data
    pthread_mutex_t *forks;            // Array of mutexes representing forks
    t_philo         *philos;           // Array of philosopher data structures
} t_data;


#endif