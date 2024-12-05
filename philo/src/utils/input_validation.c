/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 20:51:30 by abouguri          #+#    #+#             */
/*   Updated: 2024/12/05 17:12:30 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
    printf("  n_philos*:                         1-200\n");
    printf("  time_to_die* (ms):                 60+\n");
    printf("  time_to_eat* (ms):                 60+\n");
    printf("  time_to_sleep* (ms):               60+\n");
    printf("  nb_each_philo_must_eat (Optional): 0+\n");
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
