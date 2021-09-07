/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 18:28:16 by mlebrun           #+#    #+#             */
/*   Updated: 2021/09/07 15:07:38 by mlebrun          ###   ########.fr       */
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
	unsigned int				end;	
	int							*last_call;
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
	unsigned int		eat_nb;
}						t_philo;


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
		if (!data->end)
			usleep(100);
		else
			return (0);
		current_t = get_current_ts();
		slept_t = current_t - start_t;
	}
	return (1);
}

unsigned int		is_finished(t_philo *philo)
{
	if (philo->data->end)
		return (1);
	return (0);
}

int		practice_activity(t_philo *philo, long long int time, long long int time_to)
{
	long long int		time_exceed;

	if (time_to + (time - philo->last_eat) >= (long long)philo->data->time_to_die)
	{
		time_exceed = (time + (time_to - philo->last_eat)) - philo->data->time_to_die;
		if (!ft_usleep(time_to - time_exceed, philo->data))
			return (0);
		time = get_prog_time(philo);
		if (!philo->data->end)
		{
			philo->data->end = 1;
			printf("%lld %d died\n", time, philo->id);
			return (0);
		}
		else
			return (0);
	}
	else
	{
		if (!ft_usleep(time_to, philo->data))
			return (0);
	}
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
	pthread_mutex_unlock(&philo->data->lock);
	if (!philo->data->end)
	{
		printf("%lld %d has taken a fork\n", time, philo->id);
		printf("%lld %d has taken a fork\n", time, philo->id);
	}
}

void	think(t_philo *philo)
{
	long long int		time;

	time = get_prog_time(philo);
	if (!philo->data->end)
		printf("%lld %d is thinking\n", time, philo->id);
} 

int		rest(t_philo *philo)
{
	long long int		time;

	time = get_prog_time(philo);
	if (!philo->data->end)
		printf("%lld %d is sleeping\n", time, philo->id);
	else
		return (0);
	if (!practice_activity(philo, time, philo->data->time_to_sleep))
		return (0);
	return (1);
}

int		is_dead(long long int time, t_philo *philo)
{
	if ((time - philo->last_eat) > (long long)philo->data->time_to_die)
	{
		if (!philo->data->end)
		{
			philo->data->end = 1;
			printf("%lld %d died\n", time, philo->id);
		}
		return (0);
	}
	return (1);
}

int		eat(t_philo *philo)
{
	long long int		time;

	time = get_prog_time(philo);
	philo->last_eat = time;
	if (!philo->data->end)
		printf("%lld %d is eating\n", time, philo->id);
	if ((time - philo->last_eat) > (long long)philo->data->time_to_die)
	{
		if (!philo->data->end)
		{
			philo->data->end = 1;
			printf("%lld %d died\n", time, philo->id);
		}
		return (0);
	}
	else
	{
		if (!ft_usleep(philo->data->time_to_eat, philo->data))
			return (0);
	}
	philo->fork_l = 1;
	philo->fork_r = 1;
	philo->prev->fork_r = 1;
	philo->next->fork_l = 1;
	philo->eat_nb++;
	if (philo->eat_nb == philo->data->time_each_philo_must_eat)
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

	i = philo->data->nb_philo;
	while (i >= 0)
	{
		if (i != 0 && philo->data->last_call[i] == philo->id && philo->data->last_call[i - 1] != -1)
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
	unsigned long long int		i;

	i = 0;
	while (i < philo->data->nb_philo)
	{
		printf("%d ", philo->data->last_call[i]);
		i++;
	}
	printf("\n");
}

void	*test(void *data_philo)
{
	t_philo				*philo;
	int					died;
	long long int		time;


	philo = (t_philo*)data_philo;
	died = 0;
	if (philo->data->nb_philo == 1)
	{
		time = get_prog_time(philo);
		printf("%lld %d has taken a fork\n", time, philo->id);
		ft_usleep(philo->data->time_to_die, philo->data);
		time = get_prog_time(philo);
		printf("%lld %d died\n", time, philo->id);
	}
	else
	{
		while (1)
		{
			while (1)
			{
				pthread_mutex_lock(&philo->data->lock);
				if (philo->fork_l && philo->fork_r && can_eat(philo))
				{
					update_eat_status(philo);
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
			}
			if (died)
				break ;
			if (!rest(philo))
				break ;
			think(philo);
		}
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
	philo->eat_nb = 0;
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
	data->th = NULL;
	data->last_call = NULL;
	data->end = 0;
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
	pthread_mutex_destroy(&(data->lock));
	free(data->th);
	free(data->last_call);
	free(data);
}

void	free_philo(t_philo *philo, unsigned long long int nb_philo)
{
	t_philo		*prev;
	unsigned int		i;

	i = 0;
	while (i < nb_philo)
	{
		prev = philo;
		philo = philo->next;
		free(prev);
		i++;
	}
}

void	free_content(t_data *data, t_philo *philo)
{
	free_philo(philo, data->nb_philo);
	free_data(data);
}

int	main(int argc, char **argv)
{
	
	int					i;
	struct timeval		tp;
	long long int		first_ts;
	t_philo				*first_philo;
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
	i = 0;
	while (i < (int)data->nb_philo)
	{
		pthread_join(data->th[i], NULL);
		i++;
	}
	free_content(data, first_philo);
	return (0);
}
