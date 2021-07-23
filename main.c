#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

int		fork_nb = 3;

void	*test(void *data)
{
	return ("YES");
}

int	main(int argc, char **argv)
{
	pthread_t th[3];
	int			i;

	(void)argv;
	i = 0;
	if (argc < 5 || argc > 6)
		printf("Error arguments: NUMBER_OF_PHILOSOPHERS TIME_TO_DIE TIME_TO_EAT TIME_TO_SLEEP [NUMBER_OF_TIMES_EACH_PHILOSOPHER_MUST_EAT]\n");
	while  (i < 3)
	{
		if (i == 0)
			pthread_create(&th[i], NULL, &test, "oui");
		else if (i == 1)
			pthread_create(&th[i], NULL, &test, "non");
		else if (i == 2)
			pthread_create(&th[i], NULL, &test, "ptet");
		i++;
	}

	pthread_join(th[0], NULL);
	pthread_join(th[1], NULL);
	pthread_join(th[2], NULL);
	printf("this is the end\n");
	return (0);
}

