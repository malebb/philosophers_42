/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 09:13:50 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/25 10:11:06 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threads.h"

unsigned int	init_threads(t_data *data, t_philo **philos)
{
	unsigned int	i;

	data->th = malloc(sizeof(pthread_t) * data->nb_philo);
	if (!data->th)
	{
		free_data(data);
		return (0);
	}
	create_threads(philos, data);
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(data->th[i], NULL);
		i++;
	}
	pthread_join(data->checker, NULL);
	return (1);
}

void	*routine(void *data)
{
	t_philo				*philo;

	philo = (t_philo *) data;
	if (philo->data->nb_philo == 1)
	{
		think(philo);
		printf("%lld %d has taken a fork\n", get_prog_time(philo), philo->id);
	}
	else
	{
		while (!philo->data->end && !is_satiate(philo->data))
		{
			think(philo);
			take_fork(philo);
			eat(philo);
			pthread_mutex_unlock(philo->r_fork);
			pthread_mutex_unlock(philo->l_fork);
			if (!rest(philo))
				break ;
		}
	}
	return (NULL);
}

static int	is_dead(long long int time, t_philo *philo)
{
	if ((time - philo->last_eat) > (long long) philo->data->time_to_die)
		return (0);
	return (1);
}

void	*checker(void *data)
{
	t_philo				**philos;
	unsigned int		i;
	long long int		time;
	int					end;

	philos = (t_philo **) data;
	end = 0;
	while (!end && !is_satiate(philos[0]->data))
	{
		i = 0;
		while (i < philos[0]->data->nb_philo)
		{
			time = get_prog_time(philos[i]);
			if (!is_dead(time, philos[i]))
			{
				philos[i]->data->end = 1;
				end = 1;
				if (!is_satiate(philos[i]->data))
					printf("%lld %d died\n", time, philos[i]->id);
				break ;
			}
			i++;
		}
	}
	return (NULL);
}
