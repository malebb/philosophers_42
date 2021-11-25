/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 10:00:06 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/25 13:40:02 by mlebrun          ###   ########.fr       */
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

void	free_content(t_data *data, t_philo **philos)
{
	free_philos(philos, data->nb_philo);
	free(data->forks);
	free(data->th);
	free(data);
}
