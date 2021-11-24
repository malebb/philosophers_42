/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 09:05:59 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/24 10:48:47 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_H
# define INIT_H
# include "philosophers.h"
# include "threads.h"

void			init_philo(t_philo **philo, int id, t_data *data);
t_data			*init_data(long long int first_ts);
unsigned int	init_mutexes(t_philo **philos, t_data *data, unsigned int i);
unsigned int	create_threads(t_philo **philos, t_data *data);
#endif
