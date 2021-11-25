/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clock.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 14:42:39 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/25 14:00:51 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include "utils.h"

long long int	get_prog_time(t_philo *philo)
{
	struct timeval		tp;

	gettimeofday(&tp, NULL);
	return ((tp.tv_sec * 1000 + (tp.tv_usec / 1000) - philo->data->first_ts));
}

long long int	get_current_ts(void)
{
	struct timeval		tp;

	gettimeofday(&tp, NULL);
	return ((tp.tv_sec * 1000 + tp.tv_usec / 1000));
}

int	ft_usleep(unsigned long long int time, t_data *data)
{
	unsigned long long int		slept_t;
	unsigned long long int		start_t;
	long long int				current_t;

	slept_t = 0;
	start_t = get_current_ts();
	while (slept_t < time)
	{
		if (!data->end && !is_satiate(data))
			usleep(10);
		else
			return (0);
		current_t = get_current_ts();
		slept_t = current_t - start_t;
	}
	return (1);
}
