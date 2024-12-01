/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukia <rukia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:30:55 by abouguri          #+#    #+#             */
/*   Updated: 2024/12/01 18:04:10 by rukia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

is_input_numeric(int ac, char **av)
{
    int i;
    int j;
    
    i = 1;
    while(i < ac)
    {
        j = 0;
        while(av[i][j])
        {
            if (av[i][j] < '0' || av[i][j] > '9')
                return (ERROR_WRONG_INPUT);
            j++;
        }
        i++;
    }
    return (SUCCESS);
}

int validate_arguments(int ac,char  **av)
{
    if (ac < 5 || ac > 6)
		return (ERROR_WRONG_INPUT);
	if (is_input_numeric(ac, av) != 0)
		return (ERROR_WRONG_INPUT);
	if (wrong_input_check(ac, av))
		return (ERROR_WRONG_INPUT);
	return (SUCCESS);
}

void    display_help_message()
{

}

int initialize_simulation(int ac, char **av)
{
    
}

int main(int ac, char **av)
{
    if (validate_arguments(ac, av) != 0)
	{
		display_help_message();
		return (ERROR_WRONG_INPUT);
	}
	if (initialize_simulation(ac, av) != 0)
		return (ERROR_MALLOC_FAILURE);
    return (SUCCESS);
}
