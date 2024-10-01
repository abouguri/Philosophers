#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <pthread.h>
#include <sys/time.h>

typedef struct s_params {
    int num_philosophers;       // Number of philosophers and forks
    int time_to_die;            // Time in milliseconds before a philosopher dies if they don't eat
    int time_to_eat;            // Time in milliseconds it takes for a philosopher to eat
    int time_to_sleep;          // Time in milliseconds a philosopher spends sleeping
    int num_times_to_eat;       // Number of times each philosopher must eat (optional)
    struct timeval start_time;  // Simulation start time
    pthread_mutex_t print_lock;
} t_params;

typedef struct s_philosopher {
    int id;                     // Philosopher ID (index)
    int meals_eaten;            // Number of meals eaten by this philosopher
    pthread_t thread;           // Thread associated with the philosopher
    pthread_t monitor_thread;          //llll
    pthread_mutex_t *left_fork; // Pointer to the mutex of the left fork
    pthread_mutex_t *right_fork;// Pointer to the mutex of the right fork
    struct timeval last_meal_time; // Time of the last meal
    t_params *params;           // Pointer to simulation parameters
} t_philosopher;

typedef struct s_simulation {
    t_params *params;           // Pointer to simulation parameters
    t_philosopher *philosophers;// Array of philosophers
    pthread_mutex_t *forks;     // Array of mutexes representing forks
    int simulation_ended;       // Flag to indicate if the simulation has ended
    pthread_mutex_t print_lock; // Mutex to protect printing to the console
} t_simulation;


// Function prototypes
void *philosopher_routine(void *arg);
void eat(t_philosopher *philosopher);
void think(t_philosopher *philosopher);
void sleep_philosopher(t_philosopher *philosopher);
void monitor_philosophers(t_simulation *sim);
// void *monitor_philosophers(void *arg);
long get_time_diff(struct timeval start, struct timeval end);
void print_status(t_philosopher *philosopher, const char *status);

#endif