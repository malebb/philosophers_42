/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 18:28:16 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/23 11:12:53 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#define KRED  "\x1B[31m"
#define KWHT  "\x1B[37m"

typedef struct			s_data
{
	pthread_t					*th;
	pthread_t					checker;
	pthread_mutex_t				*forks;
	long long int				first_ts;
	int							eaten_id;
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
	int					sleeping;
	int					thinking;
	long long int		last_eat;
	unsigned int		eat_nb;

}						t_philo;

unsigned int		is_satiate(t_data *data)
{
	if (data->all_satiate >= data->nb_philo)
		return (1);
	return (0);
}

long long int		get_prog_time(t_philo *philo)
{
	struct timeval		tp;

	gettimeofday(&tp, NULL);
	return ((tp.tv_sec * 1000 + (tp.tv_usec / 1000) - philo->data->first_ts));
}

unsigned long long int		get_current_ts(void)
{

	struct timeval		tp;

	gettimeofday(&tp, NULL);
	return ((tp.tv_sec * 1000 + tp.tv_usec / 1000));
}

int	ft_usleep(unsigned long long int time, t_data *data)
{
	unsigned long long int		slept_t;
	unsigned long long int		start_t;
	unsigned long long int		current_t;

	slept_t = 0;
	start_t = get_current_ts();
	while (slept_t < time)
	{
		if (!data->end && !is_satiate(data))
			usleep(10);
		else
				return (0);
		current_t = get_current_ts();
		slept_t = current_t - start_t;
	}
	return (1);
}

int		practice_activity(t_philo *philo, long long int time_to)
{
	if (!ft_usleep(time_to, philo->data))
		return (0);
	return (1);
}

void	take_fork(t_philo *philo)
{
	if ((philo->id % 2) == 0)
	{
		pthread_mutex_lock(philo->l_fork);
		if (!philo->data->end && !is_satiate(philo->data))
			printf("%lld %d has taken a fork\n", get_prog_time(philo), philo->id);
		pthread_mutex_lock(philo->r_fork);
	}
	else
	{
		pthread_mutex_lock(philo->r_fork);
		if (!philo->data->end && !is_satiate(philo->data))
			printf("%lld %d has taken a fork\n", get_prog_time(philo), philo->id);
		pthread_mutex_lock(philo->l_fork);
	}
	if (!philo->data->end && !is_satiate(philo->data))
		printf("%lld %d has taken a fork\n", get_prog_time(philo), philo->id);
}

int		eat(t_philo *philo)
{
	long long int		time;

	time = get_prog_time(philo);
	philo->last_eat = time;
	if (!philo->data->end && !is_satiate(philo->data))
		printf("%s%lld %d is eating%s\n", KRED, time, philo->id, KWHT);
	else
		return (0);
	if (!practice_activity(philo, philo->data->time_to_eat))
		return (0);
	philo->eat_nb++;
	if (philo->eat_nb == philo->data->time_each_philo_must_eat)
		philo->data->all_satiate++;
	return (1);
}

void	think(t_philo *philo)
{
	if (!philo->data->end && !is_satiate(philo->data))
		printf("%lld %d is thinking\n", get_prog_time(philo), philo->id);
} 

int		rest(t_philo *philo)
{
	long long int		time;

	time = get_prog_time(philo);
	if (!philo->data->end && !is_satiate(philo->data))
		printf("%lld %d is sleeping\n", time, philo->id);
	else
		return (0);
	if (!practice_activity(philo, philo->data->time_to_sleep))
		return (0);
	return (1);
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

int	can_eat(t_philo *philo)
{
	int		i;

	i = philo->data->nb_philo - 1;
	while (i >= 0)
	{
		if (i != 0 && philo->data->last_call[i] == philo->id && philo->data->last_call[i - 1] == -1)
			return (0);
		else if ((i == 0 && philo->data->last_call[i] == philo->id) || (philo->data->last_call[i] != philo->id && i != 0 && philo->data->last_call[i - 1] == -1))
			return (1);
		i--;
	}
	return (1);
}
void	update_eat_status(t_philo *philo)
{
	int		i;

	i = 0;
	while (i < (int)philo->data->nb_philo)
	{
		if (i != (int)philo->data->nb_philo - 1)
			philo->data->last_call[i] = philo->data->last_call[i + 1];
		else
			philo->data->last_call[i] = philo->id;
		i++;
	}
}

void	print_status(t_philo *philo)
{
	unsigned int	i;

	i = 0;
	while (i < philo->data->nb_philo)
	{
		printf("%d ", philo->data->last_call[i]);
		i++;
	}
	printf("\n");
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
	(*philo)->sleeping = 0;
	(*philo)->thinking = 0;
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

int	main(int argc, char **argv)
{
	
	unsigned int		i;
	struct timeval		tp;
	long long int		first_ts;
	t_philo				**philos;
	t_data				*data;

	(void)argc;
	(void)argv;
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
		i++;
	}
	pthread_create(&data->checker, NULL, &checker, philos);
	i = 0;
	while  (i < data->nb_philo)
	{
		pthread_create(&(data->th[i]), NULL, &routine, philos[i]);
		i++;
	}
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
