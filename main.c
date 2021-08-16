/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 18:28:16 by mlebrun           #+#    #+#             */
/*   Updated: 2021/08/16 13:35:21 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


typedef struct			s_data
{
	pthread_t					*th;
	long long int				first_ts;
	pthread_mutex_t				lock;
	int							eaten_id;
	unsigned long long int		nb_philo;
	unsigned long long int		time_to_die;
	unsigned long long int		time_to_eat;
	unsigned long long int		time_to_sleep;
	unsigned long long int		time_each_philo_must_eat;
}						t_data;

typedef	struct			s_philo
{
	t_data				*data;
	int 				id;
	int					fork_r;
	int					fork_l;
	int					sleeping;
	int					thinking;
	struct	s_philo		*next;
	struct	s_philo		*prev;
	int					last;
	long long int		last_eat;
}						t_philo;

long long int		get_prog_time(t_philo *philo)
{
	struct timeval		tp;

	gettimeofday(&tp, NULL);
	return (((tp.tv_sec * 1000000 + tp.tv_usec) - philo->data->first_ts) / 1000);
}

int		practice_activity(t_philo *philo, long long int time, long long int time_to)
{
	long long int		time_exceed;

	if ((time + time_to) >= (long long)philo->data->time_to_die)
	{
		time_exceed = ((time + time_to)) - philo->data->time_to_die;
//		printf("ID = %d | %lld | %lld\n", philo->id, time_exceed, time_to);
		usleep((((time + time_to) - philo->last_eat) - time_exceed) * 1000);
		time = get_prog_time(philo);
		printf("%lld %d died\n", time, philo->id);
		return (0);
	}
	else
		usleep(philo->data->time_to_eat * 1000);
	return (1);
}

void	take_fork(t_philo *philo)
{
	long long int		time;

	time = get_prog_time(philo);
	philo->fork_l = 0;
	philo->fork_r = 0;
	philo->prev->fork_r = 0;
	philo->next->fork_l = 0;
//	printf("JUST CHANGE prev = %d R = %d L = %d\n", philo->prev->id, philo->prev->fork_r, philo->prev->fork_l);
	pthread_mutex_unlock(&philo->data->lock);
	printf("%lld %d has taken a fork\n", time, philo->id);
}

void	think(t_philo *philo)
{
	long long int		time;

	time = get_prog_time(philo);
	printf("%lld %d is thinking\n", time, philo->id);
} 
int		rest(t_philo *philo)
{
	long long int		time;

	time = get_prog_time(philo);
	printf("%lld %d is sleeping\n", time, philo->id);
	if (!practice_activity(philo, time, philo->data->time_to_sleep))
		return (0);
	return (1);
}

int		is_dead(long long int time, t_philo *philo)
{
	if ((time - philo->last_eat) > (long long)philo->data->time_to_die)
	{
		printf("%lld %d died\n", time, philo->id);
		return (0);
	}
	return (1);
}
int		eat(t_philo *philo)
{
	long long int		time;

	time = get_prog_time(philo);
	printf("%lld %d is eating\n", time, philo->id);
//	printf("id = %d | sleep time in micro = %llu\n", philo->id, (philo->data->time_to_eat * 1000));
	if (!practice_activity(philo, time, philo->data->time_to_eat))
		return (0);
	philo->fork_l = 1;
	philo->fork_r = 1;
	philo->prev->fork_r = 1;
	philo->next->fork_l = 1;
	time = get_prog_time(philo);
	philo->last_eat = time;
	printf("philo %d has eaten\n", philo->id);
	return (1);
}

void	*test(void *data_philo)
{
	t_philo				*philo;
	int					died;

	philo = (t_philo*)data_philo;
	died = 0;
	while (1)
	{
		while (1)
		{
			pthread_mutex_lock(&philo->data->lock);
		//printf("AFTER CHANGE prev = %d R = %d L = %d\n", philo->prev->id, philo->prev->fork_r, philo->prev->fork_l);
			if (philo->fork_l && philo->fork_r)
			{
				take_fork(philo);
				if (!eat(philo))
					died = 1;
				break ;
			}
			else
				pthread_mutex_unlock(&philo->data->lock);
			if (!is_dead(get_prog_time(philo), philo))
			{
				died = 1;
				break ;
			}
			//printf("coucou\n");
		}
		if (died)
			break ;
		if (!rest(philo))
			break ;
		think(philo);
	}
	return (NULL);
}

t_philo		*create_philo(int id)
{
	t_philo		*philo;

	philo = malloc(sizeof(t_philo) * (1));
	if (!philo)
		return (NULL);
	philo->id = id;
	philo->fork_r = 1;
	philo->fork_l = 1;
	philo->sleeping = 0;
	philo->thinking = 0;
	philo->next = NULL;
	philo->prev = NULL;
	philo->last = 1;
	return (philo);
}

t_philo		*add_philo(t_philo **first_philo, int id)
{
	t_philo		*first;
	t_philo		*new_philo;

	new_philo = create_philo(id);
	if (!new_philo)
		return (NULL);
	if (!(*first_philo))
	{
		*first_philo = new_philo;
		(*first_philo)->next = *first_philo;
		(*first_philo)->prev = *first_philo;
	}
	else
	{
		first = *first_philo;
		first->prev = new_philo;
		while ((*first_philo)->last != 1)
			(*first_philo) = (*first_philo)->next;
		(*first_philo)->next = new_philo;
		(*first_philo)->last = 0;
		new_philo->prev = (*first_philo);
		new_philo->next = first;
		*first_philo = first;
	}
	return (*first_philo);
}

int		create_n_philo(t_philo **first_philo, int n)
{
	int		i;

	i = 0;
	while (i < n)
	{
		if (!add_philo(first_philo, i + 1))
			return (0);
		i++;
	}
	return (1);
}

void	init_philo(t_philo *philo, int id, t_data *data)
{
	philo->data = data;
	philo->id = id;
	philo->fork_r = 1;
	philo->fork_l = 1;
	philo->sleeping = 0;
	philo->thinking = 0;
	philo->last_eat = 0;
}

t_philo		*n_philo(t_philo *philo, int n)
{
	int		i;

	i = 0;
	while (i < n)
	{
		philo = philo->next;
		i++;
	}
	return (philo);
}

t_data		*init_data(long long int first_ts)
{
	t_data		*data;

	data = malloc(sizeof(t_data) * (1));
	if (!data)
		return (NULL);
	data->first_ts = first_ts;
	if (pthread_mutex_init(&(data->lock), NULL))
		return (NULL);
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

int		is_number(char *argv, int index)
{
	int		i;

	i = 0;
	while (argv[i] != '\0')
	{
		if ((!is_digit(argv[i]) && argv[i] != '-') || (argv[i] == '-' && i != 0) || (argv[i] == '-' && argv[i + 1] == '\0'))
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
		if (!is_number(argv[i], i))
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

int	main(int argc, char **argv)
{
	int					i;
	struct timeval		tp;
	long long int		first_ts;
	t_philo				*first_philo;
	t_data				*data;


	(void)argv;
	gettimeofday(&tp, NULL);
	first_ts = tp.tv_sec * 1000000 + tp.tv_usec;
	if (argc < 5 || argc > 6)
	{
		printf("Error arguments: NUMBER_OF_PHILOSOPHERS TIME_TO_DIE TIME_TO_EAT TIME_TO_SLEEP [NUMBER_OF_TIMES_EACH_PHILOSOPHER_MUST_EAT]\n");
		return (1);
	}
	data = init_data(first_ts);
	parse_arg(argv, argc, data);
	data->th = malloc(sizeof(t_data) * data->nb_philo);
	if (!data->th)
		return (1);
	printf("nb of philo = %llu\n", data->nb_philo);
	printf("time to die = %llu\n", data->time_to_die);
	printf("time to eat = %llu\n", data->time_to_eat);
	printf("time to sleep = %llu\n", data->time_to_sleep);
	printf("time each philo must eat = %llu\n", data->time_each_philo_must_eat);
	first_philo = NULL;
	create_n_philo(&first_philo, data->nb_philo);
	i = 1;
	while  (first_philo->last != 1)
	{
		init_philo(first_philo, i, data);
		pthread_create(&(data->th[i - 1]), NULL, &test, first_philo);
		first_philo = first_philo->next;
		i++;
	}
	init_philo(first_philo, i, data);
	pthread_create(&(data->th[i - 1]), NULL, &test, first_philo);
	first_philo = first_philo->next;
	pthread_join(data->th[0], NULL);
	pthread_join(data->th[1], NULL);
	pthread_join(data->th[2], NULL);
	return (0);
}
