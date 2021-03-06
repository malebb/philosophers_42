/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:35:32 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/26 11:04:38 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "init.h"
#include "utils.h"

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

t_data	*init_data(int argc)
{
	t_data				*data;

	if (argc < 5 || argc > 6)
	{
		printf("Error arguments: NUMBER_OF_PHILOSOPHERS TIME_TO_DIE \
TIME_TO_EAT TIME_TO_SLEEP [NUMBER_OF_TIMES_EACH_PHILOSOPHER_\
MUST_EAT]\n");
		return (NULL);
	}
	data = malloc(sizeof(t_data) * (1));
	if (!data)
		return (NULL);
	data->end = 0;
	data->all_satiate = 0;
	data->time_each_philo_must_eat = -1;
	data->forks = NULL;
	data->th = NULL;
	return (data);
}

void	init_fork_mutexes(t_philo **philos, t_data *data, unsigned int i)
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

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_create(&(data->th[i]), NULL, &routine, philos[i]);
		i++;
	}
	pthread_create(&data->checker, NULL, &checker, philos);
}

t_philo	**init_data_philo(t_data *data)
{
	unsigned int	i;
	t_philo			**philos;

	data->forks = malloc((sizeof(pthread_mutex_t) * data->nb_philo));
	if (!data->forks)
		return (0);
	philos = malloc((sizeof(t_philo *) * data->nb_philo));
	if (!philos)
		return (0);
	setup_philos(philos, data);
	i = 0;
	while (i < data->nb_philo)
	{
		if (!init_philo(&(philos[i]), i, data))
			return (0);
		init_fork_mutexes(philos, data, i);
		i++;
	}
	pthread_mutex_init(&data->end_lock, NULL);
	pthread_mutex_init(&data->last_eat_lock, NULL);
	pthread_mutex_init(&data->all_satiate_lock, NULL);
	return (philos);
}
