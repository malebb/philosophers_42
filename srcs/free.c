/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 10:00:06 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/25 10:00:18 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "free.h"

void	free_data(t_data *data)
{
	free(data->th);
	free(data->last_call);
	free(data);
}

void	free_content(t_data *data, t_philo **philo)
{
	(void)philo;
	free_data(data);
}
