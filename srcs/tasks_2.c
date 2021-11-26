/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 10:17:38 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/26 10:25:50 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks.h"
#include "utils.h"

void	take_fork_even(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	pthread_mutex_lock(&philo->data->end_lock);
	if (!philo->data->end && !is_satiate(philo->data))
		printf("%lld %d has taken a fork\n", get_prog_time(philo),
			philo->id);
	pthread_mutex_unlock(&philo->data->end_lock);
	pthread_mutex_lock(philo->r_fork);
}
