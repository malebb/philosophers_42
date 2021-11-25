/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 11:04:14 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/25 11:08:03 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H
# include "philosophers.h"

unsigned int				is_satiate(t_data *data);
unsigned long long int		ft_atoi(char *nb);
unsigned int				is_digit(char c);

#endif
