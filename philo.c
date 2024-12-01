/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukia <rukia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 13:30:55 by abouguri          #+#    #+#             */
/*   Updated: 2024/12/01 17:52:56 by rukia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int validate_arguments(ac, av)
{

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
