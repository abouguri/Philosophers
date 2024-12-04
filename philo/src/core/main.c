/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukia <rukia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 20:19:17 by rukia             #+#    #+#             */
/*   Updated: 2024/12/04 05:12:39 by rukia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
