/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clock.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 15:22:18 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/24 13:33:02 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLOCK_H
# define CLOCK_H
# include "philosophers.h"

long long int				get_prog_time(t_philo *philo);
unsigned long long int		get_current_ts(void);
int							ft_usleep(unsigned long long int time,
								t_data *data);

#endif
