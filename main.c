#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NB_PHILO 3
#define NB_FORK 3

typedef	struct			s_philo
{
	//global info
	struct timeval		tp;
	int					fork_nb;
	long long int		first_ts;

	//philo info
	int 				id;
	int					fork;
	int					sleeping;
	int					thinking;
	struct	s_philo		*next;
}						t_philo;


void	*test(void *data_philo)
{
	long long int		time;
	int					id;
	t_philo				*philo;

	(void)time;
	philo = (t_philo*)data_philo;
	id = data->current_id;
	printf("current_id = %d\n", id);
/*
	(void)data;
	while (1)
	{
		gettimeofday(&(data->tp), NULL);
		if (data->fork_nb >= 2)
		{
			time = ((data->tp.tv_sec * 1000000 + data->tp.tv_usec) - data->first_ts) / 1000;
			printf("%lld, %d has taken a fork\n", time, data->first_philo->id);
		}
	}
	sleep(5);
	printf("nickel\n");
	*/
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
		if (!add_philo(&first_philo, i + 1))
			return (0);
		i++;
	}
	return (1);
}

int		init_philo()
{

}

int	main(int argc, char **argv)
{
	pthread_t 			th[NB_PHILO];
	t_philo				philo[NB_PHILO];
	int					i;
	struct timeval		tp;
	long long int		first_ts;
	t_philo				*first_philo;


	(void)argv;
	gettimeofday(&tp, NULL);
	first_ts = tp.tv_sec * 1000000 + tp.tv_usec;
	i = 0;
	if (argc < 5 || argc > 6)
		printf("Error arguments: NUMBER_OF_PHILOSOPHERS TIME_TO_DIE TIME_TO_EAT TIME_TO_SLEEP [NUMBER_OF_TIMES_EACH_PHILOSOPHER_MUST_EAT]\n");
	first_philo = NULL;
	create_n_philo(&first_philo, 5);
	while  ()
	{
		data.current_id = i + 1;
		init_philo(philo[i]);
		pthread_create(&th[i], NULL, &test, philo[i]);
		printf("coucou\n");
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
