/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 11:04:14 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/26 10:24:33 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H
# include "philosophers.h"

unsigned int				is_satiate(t_data *data);
unsigned long long int		ft_atoi(char *nb);
unsigned int				is_digit(char c);
void						setup_philos(t_philo **philos, t_data *data);
long long int				update_last_eat(t_philo *philo);

#endif
