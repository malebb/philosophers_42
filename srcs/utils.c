/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 11:00:15 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/26 10:18:38 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clock.h"
#include "utils.h"

unsigned int	is_satiate(t_data *data)
{
	pthread_mutex_lock(&data->all_satiate_lock);
	if (data->all_satiate >= data->nb_philo)
	{
		pthread_mutex_unlock(&data->all_satiate_lock);
		return (1);
	}
	pthread_mutex_unlock(&data->all_satiate_lock);
	return (0);
}

unsigned long long int	ft_atoi(char *nb)
{
	unsigned long long int	nbr;
	int						i;

	i = 0;
	nbr = 0;
	while (nb[i] != '\0')
	{
		nbr *= 10;
		nbr += (nb[i] - '0');
		i++;
	}
	return (nbr);
}

unsigned int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

void	setup_philos(t_philo **philos, t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		philos[i] = NULL;
		i++;
	}
}

long long int	update_last_eat(t_philo *philo)
{
	long long int		time;

	time = get_prog_time(philo);
	pthread_mutex_lock(&philo->data->last_eat_lock);
	philo->last_eat = time;
	pthread_mutex_unlock(&philo->data->last_eat_lock);
	return (time);
}
