#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

typedef	struct			s_philo
{
	int 				id;
	int					fork;
	int					sleeping;
	int					thinking;
	struct	s_philo		*next;
}						t_philo;

typedef	struct			s_data
{
	int					fork_nb;
	long long int		first_ts;
	struct timeval		tp;
	t_philo				*first_philo;
}						t_data;


void	*test(void *data_philo)
{
	long long int		time;
	t_data				*data;

	data = (t_data*)data_philo;
	while (1)
	{
		gettimeofday(&(data->tp), NULL);
		if (data->fork_nb >= 2)
		{
			time = ((data->tp.tv_sec * 1000000 + data->tp.tv_usec) - data->first_ts) / 1000;
			printf("%lld, %d has taken a fork\n", time, data->first_philo->id);
		}
	}
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

int		create_n_philo(t_data *data, int n)
{
	int		i;

	i = 0;
	while (i < n)
	{
		if (!add_philo(&(data->first_philo), i + 1))
			return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	pthread_t th[3];
	int			i;
	t_data		data;

	(void)argv;
	gettimeofday(&(data.tp), NULL);
	data.first_ts = data.tp.tv_sec * 1000000 + data.tp.tv_usec;
	data.first_philo = NULL;
	i = 0;
	if (argc < 5 || argc > 6)
		printf("Error arguments: NUMBER_OF_PHILOSOPHERS TIME_TO_DIE TIME_TO_EAT TIME_TO_SLEEP [NUMBER_OF_TIMES_EACH_PHILOSOPHER_MUST_EAT]\n");
	create_n_philo(&data, 5);
	data.fork_nb = 3;
	while  (i < 3)
	{
		if (i == 0)
			pthread_create(&th[i], NULL, &test, &data);
//		else if (i == 1) //			pthread_create(&th[i], NULL, &test, "2"); //		else if (i == 2)
//			pthread_create(&th[i], NULL, &test, "3");
		i++;
	}
	pthread_join(th[0], NULL);
//	pthread_join(th[1], NULL);
//	pthread_join(th[2], NULL);
	printf("this is the end\n");
	return (0);
}
