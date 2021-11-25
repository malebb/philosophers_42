/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 10:00:06 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/25 15:47:14 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "free.h"

void	free_philos(t_philo **philos, unsigned int nb_philo)
{
	unsigned int	i;

	if (philos)
	{
		i = 0;
		while (i < nb_philo)
		{
			free(philos[i]);
			i++;
		}
	}
	free(philos);
}

void	destroy_fork_mutexes(t_data *data, t_philo **philos)
{
	unsigned int	i;

	i = 0;
	if (philos)
	{
		while (i < data->nb_philo)
		{
			if (i == 0)
			{
				pthread_mutex_destroy(philos[i]->l_fork);
				pthread_mutex_destroy(philos[i]->r_fork);
			}
			else if (i != (data->nb_philo - 1))
			{
				pthread_mutex_destroy(philos[i]->r_fork);
			}
			i++;
		}
	}
}

void	free_content(t_data *data, t_philo **philos)
{
	destroy_fork_mutexes(data, philos);
	pthread_mutex_destroy(&data->end_lock);
	free_philos(philos, data->nb_philo);
	free(data->forks);
	free(data->th);
	free(data);
}
