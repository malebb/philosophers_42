#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

void	*test()
{
		printf("yo la zone\n");
		return ("YES");
}

int	main(int argc, char **argv)
{
	pthread_t t1, t2;

	(void)argv;
	if (argc < 5 || argc > 6)
		printf("Error arguments: NUMBER_OF_PHILOSOPHERS TIME_TO_DIE TIME_TO_EAT TIME_TO_SLEEP [NUMBER_OF_TIMES_EACH_PHILOSOPHER_MUST_EAT]\n");
	pthread_create(&thread_id, NULL, &test, "coucou");
	pthread_join(thread_id, NULL);
	printf("this is the end\n");
	return (0);
}

