/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 10:52:09 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/25 10:55:00 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H
# include "philosophers.h"

unsigned int	is_positive(char *argv, unsigned int i, int index);
unsigned int	is_valid(char *argv, unsigned int i, int index);
int				check_numbers(char *argv, int index);
int				parse_arg(char **argv, int argc, t_data *data);

#endif
