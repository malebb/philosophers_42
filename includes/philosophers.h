/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 13:24:33 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/23 14:36:14 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef				PHILOSOPHERS_H
#define				PHILOSOPHERS_H

#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct			s_data
{
	pthread_t					*th;
	pthread_t					checker;
	pthread_mutex_t				*forks;
	long long int				first_ts;
	unsigned int				nb_philo;
	unsigned long long int		time_to_die;
	unsigned long long int		time_to_eat;
	unsigned long long int		time_to_sleep;
	long long int				time_each_philo_must_eat;
	unsigned int				end;
	int							*last_call;
	unsigned int				all_satiate;
}						t_data;

typedef	struct			s_philo
{
	t_data				*data;
	pthread_mutex_t		*l_fork;
	pthread_mutex_t		*r_fork;
	int 				id;
	long long int		last_eat;
	unsigned int		eat_nb;
}						t_philo;

unsigned int		is_satiate(t_data *data);

#endif
