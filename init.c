/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:35:32 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/24 15:50:57 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "init.h"

unsigned int	init_philo(t_philo **philo, int id, t_data *data)
{
	*philo = malloc(sizeof(t_philo) * 1);
	if (!philo)
		return (0);
	(*philo)->data = data;
	(*philo)->id = id + 1;
	(*philo)->last_eat = 0;
	(*philo)->eat_nb = 0;
	return (1);
}

t_data	*init_data(long long int first_ts)
{
	t_data		*data;

	data = malloc(sizeof(t_data) * (1));
	if (!data)
		return (NULL);
	data->first_ts = first_ts;
	data->th = NULL;
	data->last_call = NULL;
	data->end = 0;
	data->all_satiate = 0;
	data->time_each_philo_must_eat = -1;
	return (data);
}

void	init_mutexes(t_philo **philos, t_data *data, unsigned int i)
{
	if (i == 0)
	{
		pthread_mutex_init(&philos[i]->data->forks[data->nb_philo - 1], NULL);
		philos[i]->l_fork = &philos[i]->data->forks[data->nb_philo - 1];
		pthread_mutex_init(&philos[i]->data->forks[i], NULL);
		philos[i]->r_fork = &philos[i]->data->forks[i];
	}
	else if (i == (data->nb_philo - 1))
	{
		philos[i]->l_fork = philos[i - 1]->r_fork;
		philos[i]->r_fork = philos[0]->l_fork;
	}
	else
	{
		philos[i]->l_fork = philos[i - 1]->r_fork;
		pthread_mutex_init(&philos[i]->data->forks[i], NULL);
		philos[i]->r_fork = &philos[i]->data->forks[i];
	}
}

void	create_threads(t_philo **philos, t_data *data)
{
	unsigned int	i;

	pthread_create(&data->checker, NULL, &checker, philos);
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_create(&(data->th[i]), NULL, &routine, philos[i]);
		i++;
	}
}
