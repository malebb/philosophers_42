/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 15:09:53 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/26 12:35:19 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks.h"
#include "utils.h"

unsigned int	practice_tasks(t_philo *philo)
{
	think(philo);
	if (!take_fork(philo))
		return (0);
	eat(philo);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
	if (!rest(philo))
		return (0);
	return (1);
}

unsigned int	take_fork(t_philo *philo)
{
	if ((philo->id % 2) == 0)
		take_fork_even(philo);
	else
	{
		pthread_mutex_lock(philo->r_fork);
		pthread_mutex_lock(&philo->data->end_lock);
		if (!philo->data->end && !is_satiate(philo->data))
			printf("%lld %d has taken a fork\n", get_prog_time(philo),
				philo->id);
		pthread_mutex_unlock(&philo->data->end_lock);
		if (philo->l_fork == philo->r_fork)
			return (0);
		pthread_mutex_lock(philo->l_fork);
	}
	pthread_mutex_lock(&philo->data->end_lock);
	if (!philo->data->end && !is_satiate(philo->data))
		printf("%lld %d has take a fork\n", get_prog_time(philo), philo->id);
	pthread_mutex_unlock(&philo->data->end_lock);
	return (1);
}

unsigned int	eat(t_philo *philo)
{
	long long int		time;

	time = update_last_eat(philo);
	pthread_mutex_lock(&philo->data->end_lock);
	if (!philo->data->end && !is_satiate(philo->data))
	{
		pthread_mutex_unlock(&philo->data->end_lock);
		printf("%lld %d is eating\n", time, philo->id);
	}
	else
	{
		pthread_mutex_unlock(&philo->data->end_lock);
		return (0);
	}
	if (!ft_usleep(philo->data->time_to_eat, philo->data))
		return (0);
	philo->eat_nb++;
	if (philo->eat_nb == philo->data->time_each_philo_must_eat)
	{
		pthread_mutex_lock(&philo->data->all_satiate_lock);
		philo->data->all_satiate++;
		pthread_mutex_unlock(&philo->data->all_satiate_lock);
	}
	return (1);
}

void	think(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->end_lock);
	if (!philo->data->end && !is_satiate(philo->data))
		printf("%lld %d is thinking\n", get_prog_time(philo), philo->id);
	pthread_mutex_unlock(&philo->data->end_lock);
}

unsigned int	rest(t_philo *philo)
{
	int					sleep_status;

	pthread_mutex_lock(&philo->data->end_lock);
	if (!philo->data->end && !is_satiate(philo->data))
	{
		pthread_mutex_unlock(&philo->data->end_lock);
		printf("%lld %d is sleeping\n", get_prog_time(philo), philo->id);
	}
	else
	{
		pthread_mutex_unlock(&philo->data->end_lock);
		return (0);
	}
	sleep_status = ft_usleep(philo->data->time_to_sleep, philo->data);
	return (sleep_status);
}
