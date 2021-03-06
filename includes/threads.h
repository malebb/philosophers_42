/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 09:15:54 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/25 11:15:09 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADS_H
# define THREADS_H
# include "philosophers.h"
# include "tasks.h"
# include "free.h"
# include "init.h"

void			*routine(void *data);
void			*checker(void *data);
unsigned int	init_threads(t_data *data, t_philo **philos);

#endif
