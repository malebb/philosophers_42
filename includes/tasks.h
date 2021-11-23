/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 15:17:57 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/23 15:21:41 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		TASKS_H
#define		TASKS_H

#include "clock.h"

void	take_fork(t_philo *philo);
int		eat(t_philo *philo);
void	think(t_philo *philo);
int		rest(t_philo *philo);

#endif
