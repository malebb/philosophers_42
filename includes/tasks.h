/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 15:17:57 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/26 10:20:26 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASKS_H
# define TASKS_H
# include "clock.h"

unsigned int	practice_tasks(t_philo *philo);
unsigned int	take_fork(t_philo *philo);
unsigned int	eat(t_philo *philo);
void			think(t_philo *philo);
unsigned int	rest(t_philo *philo);
void			take_fork_even(t_philo *philo);

#endif
