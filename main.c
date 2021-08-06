#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NB_PHILO 4
#define	TIME_TO_DIE 310
#define	TIME_TO_EAT 200
#define	TIME_TO_SLEEP 200

typedef struct			s_data
{
	long long int		first_ts;
	pthread_mutex_t		lock;
	int					eaten_id;
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

void	take_fork(t_philo *philo)
{
	long long int		time;

	time = get_prog_time(philo);
	philo->fork_l = 0;
	philo->fork_r = 0;
	philo->prev->fork_r = 0;
	philo->next->fork_l = 0;
	printf("%lld %d has taken a fork\n", time, philo->id);
}

void	think(t_philo *philo)
{
	long long int		time;

	time = get_prog_time(philo);
	printf("%lld %d is thinking\n", time, philo->id);
} 
void	rest(t_philo *philo)
{
	long long int		time;

	time = get_prog_time(philo);
	printf("%lld %d is sleeping\n", time, philo->id);
	usleep(TIME_TO_SLEEP * 1000);
}

int		eat(t_philo *philo)
{
	long long int		time;

	time = get_prog_time(philo);
	philo->fork_l = 1;
	philo->fork_r = 1;
	philo->prev->fork_r = 1;
	philo->next->fork_l = 1;
	if (philo->id == 1)
		printf("TIME = %lld | LAST_EAT = %lld\n", time, philo->last_eat);
	if ((time - philo->last_eat) > TIME_TO_DIE)
	{
		printf("%lld %d died\n", time, philo->id);
		return (0);
	}
	printf("%lld %d is eating\n", time, philo->id);
	philo->last_eat = time;
	usleep(TIME_TO_EAT * 1000);
	return (1);
}

void	*test(void *data_philo)
{
	t_philo				*philo;
	philo = (t_philo*)data_philo;

	while (1)
	{
		pthread_mutex_lock(&philo->data->lock);
		if (philo->fork_l && philo->fork_r)
		{
			take_fork(philo);
			if (!eat(philo))
			{
				pthread_mutex_unlock(&philo->data->lock);
				break ;
			}
			pthread_mutex_unlock(&philo->data->lock);
		}
		else
			pthread_mutex_unlock(&philo->data->lock);
		rest(philo);
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
	data = init_data(first_ts);
	i = 1;
	while  (first_philo->last != 1)
	{
		init_philo(first_philo, i, data);
		pthread_create(&th[i - 1], NULL, &test, first_philo);
		first_philo = first_philo->next;
		i++;
	}
	init_philo(first_philo, i, data);
	pthread_create(&th[i - 1], NULL, &test, first_philo);
	first_philo = first_philo->next;
	pthread_join(th[0], NULL);
	pthread_join(th[1], NULL);
	pthread_join(th[2], NULL);
	return (0);
}
