#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NB_PHILO 3
#define NB_FORK 3

typedef struct			s_data
{
	int					fork_nb;
	long long int		first_ts;
	pthread_mutex_t		lock;
}						t_data;

typedef	struct			s_philo
{
	t_data				*data;
	int 				id;
	int					fork;
	int					sleeping;
	int					thinking;
	struct	s_philo		*next;
}						t_philo;

void	*test(void *data_philo)
{
	long long int		time;
	t_philo				*philo;
	struct timeval		tp;

	philo = (t_philo*)data_philo;

	while (1)
	{
		gettimeofday(&tp, NULL);
		if (philo->data->fork_nb >= 2)
		{
			time = ((tp.tv_sec * 1000000 + tp.tv_usec) - philo->data->first_ts) / 1000;
			printf("%lld, %d has taken a fork\n", time, philo->id);
			philo->data->fork_nb-=2;
			usleep(5000000);
			philo->data->fork_nb+=2;
		}
	}
	printf("nickel\n");
	
	return ("YES");
}

t_philo		*create_philo(int id)
{
	t_philo		*philo;

	philo = malloc(sizeof(t_philo) * (1));
	if (!philo)
		return (NULL);
	philo->id = id;
	philo->fork = 0;
	philo->sleeping = 0;
	philo->thinking = 0;
	philo->next = NULL;
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
		*first_philo = new_philo;
	else
	{
		first = *first_philo;
		while ((*first_philo)->next)
			(*first_philo) = (*first_philo)->next;
		(*first_philo)->next = new_philo;
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
	philo->fork = 0;
	philo->sleeping = 0;
	philo->thinking = 0;
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

t_data		*init_data(long long int first_ts, int fork_nb)
{
	t_data		*data;

	data = malloc(sizeof(t_data) * (1));
	if (!data)
		return (NULL);
	data->first_ts = first_ts;
	data->fork_nb = fork_nb;
	pthread_mutex_init(&(data->lock));
	return (data);
}

int	main(int argc, char **argv)
{
	pthread_t 			th[NB_PHILO];
	int					i;
	struct timeval		tp;
	long long int		first_ts;
	t_philo				*first_philo;
	t_data				*data;


	(void)argv;
	gettimeofday(&tp, NULL);
	first_ts = tp.tv_sec * 1000000 + tp.tv_usec;
	if (argc < 5 || argc > 6)
		printf("Error arguments: NUMBER_OF_PHILOSOPHERS TIME_TO_DIE TIME_TO_EAT TIME_TO_SLEEP [NUMBER_OF_TIMES_EACH_PHILOSOPHER_MUST_EAT]\n");
	first_philo = NULL;
	create_n_philo(&first_philo, NB_PHILO);
	data = init_data(first_ts, NB_FORK);
	i = 1;
	while  (first_philo)
	{
		init_philo(first_philo, i, data);
		pthread_create(&th[i - 1], NULL, &test, first_philo);
//		else if (i == 1) //			pthread_create(&th[i], NULL, &test, "2"); //		else if (i == 2)
//			pthread_create(&th[i], NULL, &test, "3");
		first_philo = first_philo->next;
		i++;
	}
	pthread_join(th[0], NULL);
	pthread_join(th[1], NULL);
	pthread_join(th[2], NULL);
	printf("this is the end\n");
	return (0);
}
