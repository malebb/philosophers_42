/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 09:13:50 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/26 11:04:50 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threads.h"
#include "utils.h"

unsigned int	init_threads(t_data *data, t_philo **philos)
{
	unsigned int		i;
	struct timeval		tp;

	data->th = malloc(sizeof(pthread_t) * data->nb_philo);
	if (!data->th)
		return (0);
	gettimeofday(&tp, NULL);
	data->first_ts = tp.tv_sec * 1000 + (tp.tv_usec / 1000);
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
	pthread_mutex_lock(&philo->data->end_lock);
	while (!philo->data->end && !is_satiate(philo->data))
	{
		pthread_mutex_unlock(&philo->data->end_lock);
		if (!practice_tasks(philo))
		{
			pthread_mutex_lock(&philo->data->end_lock);
			break ;
		}
		pthread_mutex_lock(&philo->data->end_lock);
	}
	pthread_mutex_unlock(&philo->data->end_lock);
	return (NULL);
}

static int	is_dead(long long int time, t_philo *philo)
{
	pthread_mutex_lock(&philo->data->last_eat_lock);
	if ((time - philo->last_eat) > (long long) philo->data->time_to_die)
	{
		pthread_mutex_unlock(&philo->data->last_eat_lock);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->last_eat_lock);
	return (1);
}

static void	end_simulation(t_philo **philos, int *end, unsigned int i,
		long long int time)
{
	pthread_mutex_lock(&philos[i]->data->end_lock);
	philos[i]->data->end = 1;
	pthread_mutex_unlock(&philos[i]->data->end_lock);
	*end = 1;
	if (!is_satiate(philos[i]->data))
		printf("%lld %d died\n", time, philos[i]->id);
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
				end_simulation(philos, &end, i, time);
				break ;
			}
			i++;
		}
	}
	return (NULL);
}
