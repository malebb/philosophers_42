/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 10:49:53 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/25 11:14:01 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "utils.h"

unsigned int	is_positive(char *argv, unsigned int i, int index)
{
	if (argv[i] == '-' && i == 0)
	{
		if (index == 1)
			printf("Error: NB_OF_PHILO should be a positive number\n");
		else if (index == 2)
			printf("Error: TIME_TO_DIE should be a postitive number\n");
		else if (index == 3)
			printf("Error: TIME_TO_EAT should be a postitive number\n");
		else if (index == 4)
			printf("Error:  TIME_TO_SLEEP should be a postitive number\n");
		else if (index == 5)
			printf("Error:  NUMBER_OF_TIMES_EACH_PHILOSOPHER_MUST_EAT should \
be a postitive number\n");
		return (0);
	}
	return (1);
}

unsigned int	is_valid(char *argv, unsigned int i, int index)
{
	if ((!is_digit(argv[i]) && argv[i] != '-') || (argv[i] == '-' && i != 0)
		|| (argv[i] == '-' && argv[i + 1] == '\0') || (i == 0 && argv[i]
			== '0') || (i == 1 && argv[0] == '-' && argv[i] == '0'))
	{
		if (index == 1)
			printf("Error: NB_OF_PHILO is invalid\n");
		else if (index == 2)
			printf("Error: TIME_TO_DIE is invalid\n");
		else if (index == 3)
			printf("Error: TIME_TO_EAT is invalid\n");
		else if (index == 4)
			printf("Error:  TIME_TO_SLEEP is invalid\n");
		else if (index == 5)
			printf("Error:  NUMBER_OF_TIMES_EACH_PHILOSOPHER_MUST_EAT is \
invalid\n");
		return (0);
	}
	return (1);
}

int	check_numbers(char *argv, int index)
{
	unsigned int	i;

	i = 0;
	while (argv[i] != '\0')
	{
		if (!is_valid(argv, i, index))
			return (0);
		if (!is_positive(argv, i, index))
			return (0);
		i++;
	}
	return (1);
}

int	parse_arg(char **argv, int argc, t_data *data)
{
	int		i;

	i = 1;
	while (i < argc)
	{
		if (!check_numbers(argv[i], i))
			return (0);
		if (i == 1)
			data->nb_philo = ft_atoi(argv[i]);
		else if (i == 2)
			data->time_to_die = ft_atoi(argv[i]);
		else if (i == 3)
			data->time_to_eat = ft_atoi(argv[i]);
		else if (i == 4)
			data->time_to_sleep = ft_atoi(argv[i]);
		else if (i == 5)
			data->time_each_philo_must_eat = ft_atoi(argv[i]);
		i++;
	}
	return (1);
}
