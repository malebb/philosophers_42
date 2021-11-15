/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlebrun <mlebrun@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 18:28:16 by mlebrun           #+#    #+#             */
/*   Updated: 2021/11/15 16:11:57 by mlebrun          ###   ########.fr       */
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
	int							eaten_id;
	int							nb_philo;
	unsigned long long int		time_to_die;
	unsigned long long int		time_to_eat;
	unsigned long long int		time_to_sleep;
	unsigned long long int		time_each_philo_must_eat;
	unsigned int				end;
	int							*last_call;
	unsigned int				all_satiate;
}						t_data;

typedef	struct			s_philo
{
	t_data				*data;
	pthread_mutex_t		r_fork;
	pthread_mutex_t		l_fork;
	int 				id;
	int					sleeping;
	int					thinking;
	long long int		last_eat;
	unsigned int		eat_nb;
}						t_philo;

int					is_satiate(t_data *data)
{
	if (data->all_satiate)
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
	pthread_mutex_lock(&philo->l_fork);
	pthread_mutex_lock(&philo->r_fork);
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
	int			i;

	i = 0;
	while (i < philo->data->nb_philo)
	{
		printf("%d ", philo->data->last_call[i]);
		i++;
	}
	printf("\n");
}

void	*routine(void *data)
{
	t_philo				*philo;
	int					finished;
	long long int		time;


	philo = (t_philo*)data;
	finished = 0;
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
		take_fork(philo);
	}
	return (NULL);
}

void	init_philo(t_philo *philo, int id, t_data *data)
{
	philo->data = data;
	philo->id = id + 1;
	philo->sleeping = 0;
	philo->thinking = 0;
	philo->last_eat = 0;
	philo->eat_nb = 0;
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
	
	int					i;
	struct timeval		tp;
	long long int		first_ts;
	t_philo				**philos;
	t_data				*data;
	
	(void)argc;
	(void)argv;
	gettimeofday(&tp, NULL);
	first_ts = tp.tv_sec * 1000 + (tp.tv_usec / 1000);
	printf("FIRST_TS%lld\n", first_ts);
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
	philos = malloc(sizeof(t_philo) * data->nb_philo);
	if (!philos)
		return (0);
	i = 0;
	while  (i < data->nb_philo)
	{
		init_philo(philos[i], i, data);
		pthread_create(&(data->th[i]), NULL, &routine, philos);
		if (i == 0)
		{
			pthread_mutex_init(&(philos[i]->l_fork), NULL);
			pthread_mutex_init(&(philos[i]->r_fork), NULL);
		}
		else if (i == (data->nb_philo - 1))
		{
			philos[i]->l_fork = philos[i - 1]->r_fork;
			philos[i]->r_fork = philos[0]->l_fork;
			
		}
		else
		{
			philos[i]->l_fork = philos[i - 1]->r_fork;
			pthread_mutex_init(&(philos[i]->r_fork), NULL);
		}
		i++;
	}
	i = 0;
	while (i < (int)data->nb_philo)
	{
		pthread_join(data->th[i], NULL);
		i++;
	}
	free_content(data, philos);
	return (0);
}
