/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 11:00:15 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/25 11:06:44 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

unsigned int	is_satiate(t_data *data)
{
	if (data->all_satiate >= data->nb_philo)
		return (1);
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
