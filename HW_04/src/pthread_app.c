#include "defs.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

static pthread_t *philosophers;
static pthread_mutex_t *forks;

int init_forks()
{
	forks = (pthread_mutex_t *)malloc(PHILOSOPHER_COUNT *
					  sizeof(pthread_mutex_t));
	if (!forks) {
		perror("Cannot allocate forks");
		return EXIT_FAILURE;
	}

	// Initialize mutexes
	for (int i = 0; i < PHILOSOPHER_COUNT; i++) {
		pthread_mutex_init(&forks[i], NULL);
	}
}

void deinit_forks(void)
{
	for (int i = 0; i < PHILOSOPHER_COUNT; i++) {
		pthread_mutex_destroy(&forks[i]);
	}

	free(forks);
}

void *philosopher_thread(void *arg)
{
	struct Philosopher *data = (struct Philosopher *)arg;
	int id = data->id;

	int left_fork = id;
	int right_fork = (id + 1) % PHILOSOPHER_COUNT;

	int eating_cycles = 0;

	printf("Philosopher %d: Started (L=%d, R=%d)\n", id, left_fork,
	       right_fork);

	while (eating_cycles < EATING_CYCLES) {
		printf("Philosopher %d: Thinking...\n", id);
		sleep(TIME_TO_THINK);

		if (id % 2 == 0) {
			printf("Philosopher %d: Waiting for left fork %d\n", id,
			       left_fork);
			pthread_mutex_lock(&forks[left_fork]);
			printf("Philosopher %d: Picked up left fork %d\n", id,
			       left_fork);

			printf("Philosopher %d: Waiting for right fork %d\n",
			       id, right_fork);
			pthread_mutex_lock(&forks[right_fork]);
			printf("Philosopher %d: Picked up right fork %d\n", id,
			       right_fork);
		} else {
			printf("Philosopher %d: Waiting for right fork %d\n",
			       id, right_fork);
			pthread_mutex_lock(&forks[right_fork]);
			printf("Philosopher %d: Picked up right fork %d\n", id,
			       right_fork);

			printf("Philosopher %d: Waiting for left fork %d\n", id,
			       left_fork);
			pthread_mutex_lock(&forks[left_fork]);
			printf("Philosopher %d: Picked up left fork %d\n", id,
			       left_fork);
		}

		printf("Philosopher %d: EATING\n", id);
		sleep(TIME_TO_EAT);

		if (id % 2 == 0) {
			pthread_mutex_unlock(&forks[right_fork]);
			printf("Philosopher %d: Put down right fork %d\n", id,
			       right_fork);
			pthread_mutex_unlock(&forks[left_fork]);
			printf("Philosopher %d: Put down left fork %d\n", id,
			       left_fork);
		} else {
			pthread_mutex_unlock(&forks[left_fork]);
			printf("Philosopher %d: Put down left fork %d\n", id,
			       left_fork);
			pthread_mutex_unlock(&forks[right_fork]);
			printf("Philosopher %d: Put down right fork %d\n", id,
			       right_fork);
		}

		printf("Philosopher %d: Finished eating cycle %d/%d\n", id,
		       eating_cycles, EATING_CYCLES);
		eating_cycles++;
	}

	printf("Philosopher %d: Exiting after %d eating cycles\n", id,
	       eating_cycles);
	free(data);
	return NULL;
}

int main(int, char **)
{
	printf("Initializing");

	init_forks();

	philosophers =
		(pthread_t *)malloc(PHILOSOPHER_COUNT * sizeof(pthread_t));
	if (!philosophers) {
		perror("Cannot allocate philosophers");
		deinit_forks();
		return EXIT_FAILURE;
	}

	for (int i = 0; i < PHILOSOPHER_COUNT; i++) {
		struct Philosopher *data = malloc(sizeof(struct Philosopher));
		data->id = i;

		if (pthread_create(&philosophers[i], NULL, philosopher_thread,
				   data) != 0) {
			perror("Failed to create philosopher thread");
			continue;
		}
		printf("Created philosopher thread %d\n", i);
	}

	printf("Initialization complete");

	printf("Exiting\n");

	for (int i = 0; i < PHILOSOPHER_COUNT; i++) {
		pthread_join(philosophers[i], NULL);
		printf("Philosopher %d thread joined\n", i);
	}

	deinit_forks();

	printf("Exit\n");

	return 0;
}