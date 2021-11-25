/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 18:28:16 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/25 11:10:29 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include "init.h"
#include "threads.h"
#include "free.h"
#include "parse.h"

int	main(int argc, char **argv)
{
	t_philo				**philos;
	t_data				*data;

	data = init_data(argc);
	if (!data)
		return (1);
	if (!parse_arg(argv, argc, data))
	{
		free_data(data);
		return (1);
	}
	philos = init_data_philo(data);
	if (!philos)
		return (1);
	if (!init_threads(data, philos))
		return (1);
	free_content(data, philos);
	return (0);
}
