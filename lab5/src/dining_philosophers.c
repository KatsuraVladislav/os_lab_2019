#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include <Windows.h>
#define wait() _getch()

#define TABLE_SIZE 5

typedef struct philosoph_tag {
	const char *name;
	unsigned left_fork;
	unsigned right_fork;
} philosoph_t;

typedef struct table_tag {
	pthread_mutex_t forks[TABLE_SIZE];
} table_t;

typedef struct philosoph_args_tag {
	const philosoph_t *philosoph;
	const table_t *table;
} philosoph_args_t;

pthread_mutex_t critical_section = PTHREAD_MUTEX_INITIALIZER;

void init_philosoph(philosoph_t *philosoph,
	const char *name,
	unsigned left_fork,
	unsigned right_fork) {
	philosoph->name = name;
	philosoph->left_fork = left_fork;
	philosoph->right_fork = right_fork;
}

void init_table(table_t *table) {
	size_t i;
	for (i = 0; i < TABLE_SIZE; i++) {
		pthread_mutex_init(&table->forks[i], NULL);
	}
}

void* eat(void *args) {
	philosoph_args_t *arg = (philosoph_args_t*)args;
	const philosoph_t *philosoph = arg->philosoph;
	const table_t *table = arg->table;
	unsigned rand;

	do {
		printf("%s started dinner\n", philosoph->name);

		pthread_mutex_lock(&critical_section);
		pthread_mutex_lock(&table->forks[philosoph->left_fork]);
		Sleep(1000);
		pthread_mutex_lock(&table->forks[philosoph->right_fork]);
		pthread_mutex_unlock(&critical_section);

		printf("%s is eating after sleep\n", philosoph->name);

		pthread_mutex_unlock(&table->forks[philosoph->right_fork]);
		pthread_mutex_unlock(&table->forks[philosoph->left_fork]);

		printf("%s finished dinner\n", philosoph->name);
	} while (1);
}

void main() {
	pthread_t threads[TABLE_SIZE];
	philosoph_t philosophs[TABLE_SIZE];
	philosoph_args_t arguments[TABLE_SIZE];
	table_t table;
	size_t i;

	init_table(&table);

	init_philosoph(&philosophs[0], "A", 0, 1);
	init_philosoph(&philosophs[1], "B", 1, 2);
	init_philosoph(&philosophs[2], "C", 2, 3);
	init_philosoph(&philosophs[3], "D", 3, 4);
	init_philosoph(&philosophs[4], "E", 4, 0);

	for (i = 0; i < TABLE_SIZE; i++) {
		arguments[i].philosoph = &philosophs[i];
		arguments[i].table = &table;
	}

	for (i = 0; i < TABLE_SIZE; i++) {
		pthread_create(&threads[i], NULL, eat, &arguments[i]);
	}

	for (i = 0; i < TABLE_SIZE; i++) {
		pthread_join(threads[i], NULL);
	}
}
