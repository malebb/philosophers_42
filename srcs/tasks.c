/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 15:09:53 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/24 15:53:37 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks.h"

void	take_fork(t_philo *philo)
{
	if ((philo->id % 2) == 0)
	{
		pthread_mutex_lock(philo->l_fork);
		if (!philo->data->end && !is_satiate(philo->data))
			printf("%lld %d has taken a fork\n", get_prog_time(philo),
				philo->id);
		pthread_mutex_lock(philo->r_fork);
	}
	else
	{
		pthread_mutex_lock(philo->r_fork);
		if (!philo->data->end && !is_satiate(philo->data))
			printf("%lld %d has taken a fork\n", get_prog_time(philo),
				philo->id);
		pthread_mutex_lock(philo->l_fork);
	}
	if (!philo->data->end && !is_satiate(philo->data))
		printf("%lld %d has taken a fork\n", get_prog_time(philo), philo->id);
}

int	eat(t_philo *philo)
{
	long long int		time;

	time = get_prog_time(philo);
	philo->last_eat = time;
	if (!philo->data->end && !is_satiate(philo->data))
		printf("%lld %d is eating\n", time, philo->id);
	else
		return (0);
	if (!ft_usleep(philo->data->time_to_eat, philo->data))
		return (0);
	philo->eat_nb++;
	if (philo->eat_nb == philo->data->time_each_philo_must_eat)
		philo->data->all_satiate++;
	return (1);
}

void	think(t_philo *philo)
{
	if (!philo->data->end && !is_satiate(philo->data))
		printf("%lld %d is thinking\n", get_prog_time(philo), philo->id);
}

int	rest(t_philo *philo)
{
	long long int		time;
	int					sleep_status;

	time = get_prog_time(philo);
	if (!philo->data->end && !is_satiate(philo->data))
		printf("%lld %d is sleeping\n", time, philo->id);
	else
		return (0);
	sleep_status = ft_usleep(philo->data->time_to_sleep, philo->data);
	return (sleep_status);
}
