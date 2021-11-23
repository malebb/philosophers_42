/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 18:28:16 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/23 15:48:22 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include "clock.h"
#include "tasks.h"

unsigned int		is_satiate(t_data *data)
{
	if (data->all_satiate >= data->nb_philo)
		return (1);
	return (0);
}

int		is_dead(long long int time, t_philo *philo)
{
	if ((time - philo->last_eat) > (long long)philo->data->time_to_die)
		return (0);
	return (1);
}

int		init_last_call(int **last_call, unsigned int nb_philo)
{
	unsigned int	i;

	i = 0;
	*last_call = malloc((sizeof(int) * nb_philo));
	if (!(*last_call))
		return (0);
	while (i < nb_philo)
	{
		last_call[0][i] = -1;
		i++;
	}
	return (1);
}

void	*checker(void *data)
{
	t_philo				**philos;
	unsigned int		i;
	long long int		time;
	int					end;

	philos = (t_philo**)data;
	end = 0;
	while (!end && !is_satiate(philos[0]->data))
	{
		i = 0;
		while (i < philos[0]->data->nb_philo)
		{
			time = get_prog_time(philos[i]);
			if (!is_dead(time, philos[i]))
			{
				philos[i]->data->end = 1;
				end = 1; 
				if (!is_satiate(philos[i]->data))
					printf("%lld %d died\n", time, philos[i]->id);
				break ;
			}
			i++;
		}
	}
	return (NULL);
}

void	*routine(void *data)
{
	t_philo				*philo;

	philo = (t_philo*)data;
	if (philo->data->nb_philo == 1)
	{
		think(philo);
		printf("%lld %d has taken a fork\n", get_prog_time(philo), philo->id);
	}
	else
	{
		while (!philo->data->end && !is_satiate(philo->data))
		{
			think(philo);
			take_fork(philo);
			eat(philo);
			pthread_mutex_unlock(philo->r_fork);
			pthread_mutex_unlock(philo->l_fork);
			rest(philo);
		}
	}
	return (NULL);
}

void	init_philo(t_philo **philo, int id, t_data *data)
{
	*philo = malloc(sizeof(t_philo) * 1);
	if (!philo)
		return ;
	(*philo)->data = data;
	(*philo)->id = id + 1;
	(*philo)->last_eat = 0;
	(*philo)->eat_nb = 0;
}

t_data		*init_data(long long int first_ts)
{
	t_data		*data;

	data = malloc(sizeof(t_data) * (1));
	if (!data)
		return (NULL);
	data->first_ts = first_ts;
	data->th = NULL;
	data->last_call = NULL;
	data->end = 0;
	data->all_satiate = 0;
	data->time_each_philo_must_eat = -1;
	return (data);
}

unsigned long long int ft_atoi(char *nb)
{
	unsigned long long int		nbr;
	int				i;

	i = 0;
	nbr = 0;
	while (nb[i] != '\0')
	{
		nbr *= 10;
		nbr += (nb[i] - '0');
		i++;
	}
	return (nbr);
}

int		is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int		is_positive_number(char *argv, int index)
{
	int		i;

	i = 0;
	while (argv[i] != '\0')
	{
		if ((!is_digit(argv[i]) && argv[i] != '-') || (argv[i] == '-' && i != 0) || (argv[i] == '-' && argv[i + 1] == '\0') || (i == 0  && argv[i] == '0') || (i == 1 && argv[0] == '-' && argv[i] == '0'))
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
				printf("Error:  NUMBER_OF_TIMES_EACH_PHILOSOPHER_MUST_EAT is invalid\n");
			return (0); 
		}
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
				printf("Error:  NUMBER_OF_TIMES_EACH_PHILOSOPHER_MUST_EAT should be a postitive number\n");
			return (0);
		}
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
		if (!is_positive_number(argv[i], i))
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

void	free_data(t_data *data)
{
	free(data->th);
	free(data->last_call);
	free(data);
}

void	free_content(t_data *data, t_philo **philo)
{
	(void)philo;
	free_data(data);
}

int		init_mutexes(t_philo **philos, t_data *data, unsigned int i)
{
	if (i == 0)
	{
		pthread_mutex_init(&philos[i]->data->forks[data->nb_philo - 1], NULL);
		philos[i]->l_fork = &philos[i]->data->forks[data->nb_philo - 1];
		pthread_mutex_init(&philos[i]->data->forks[i], NULL);
		philos[i]->r_fork = &philos[i]->data->forks[i];
	}
	else if (i == (data->nb_philo - 1))
	{
		philos[i]->l_fork = philos[i - 1]->r_fork;
		philos[i]->r_fork = philos[0]->l_fork;
	}
	else
	{
		philos[i]->l_fork = philos[i - 1]->r_fork;
		pthread_mutex_init(&philos[i]->data->forks[i], NULL);
		philos[i]->r_fork = &philos[i]->data->forks[i];
	}
	return (1);
}

unsigned int	create_threads(t_philo **philos, t_data *data)
{
	unsigned int	i;

	pthread_create(&data->checker, NULL, &checker, philos);
	i = 0;
	while  (i < data->nb_philo)
	{
		pthread_create(&(data->th[i]), NULL, &routine, philos[i]);
		i++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	
	unsigned int		i;
	struct timeval		tp;
	long long int		first_ts;
	t_philo				**philos;
	t_data				*data;

	gettimeofday(&tp, NULL);
	first_ts = tp.tv_sec * 1000 + (tp.tv_usec / 1000);
	if (argc < 5 || argc > 6)
	{
		printf("Error arguments: NUMBER_OF_PHILOSOPHERS TIME_TO_DIE TIME_TO_EAT TIME_TO_SLEEP [NUMBER_OF_TIMES_EACH_PHILOSOPHER_MUST_EAT]\n");
		return (1);
	}
	data = init_data(first_ts);
	if (!parse_arg(argv, argc, data))
	{
		free_data(data);
		return (1);
	}
	init_last_call(&(data->last_call), data->nb_philo);
	data->th = malloc(sizeof(pthread_t) * data->nb_philo);
	if (!data->th)
	{
		free_data(data);
		return (1);
	}
	data->forks = malloc((sizeof(pthread_mutex_t) * data->nb_philo));
	philos = malloc((sizeof(t_philo *) * data->nb_philo));
	if (!philos)
		return (0);
	i = 0;
	while  (i < data->nb_philo)
	{
		init_philo(&(philos[i]), i, data);
		init_mutexes(philos, data, i);
		i++;
	}
	create_threads(philos, data);
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(data->th[i], NULL);
		i++;
	}
	pthread_join(data->checker, NULL);
	free_content(data, philos);
	return (0);
}
