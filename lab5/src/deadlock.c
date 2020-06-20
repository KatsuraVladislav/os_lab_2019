#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int X = 7;
int Y = 13;
int Z = 25;

//pthread_mutex_t mut1 = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t mut3 = PTHREAD_MUTEX_INITIALIZER;
sem_t sem1, sem2, sem3; 

void xyz() 
{
	int x;
	int y;
    int z;
	sem_wait(&sem1);
	x = X;
	printf("X = %d was put in th1\n", x);
	sleep(1);
	sem_wait(&sem2);
	y = Y;
    printf("Y = %d was put in th1\n", y);
    sem_wait(&sem3);
    z = Z;
	printf("Z = %d was put in th1\n", z);
	printf("X*Y*Z = %d\n", x * y * z);
	sem_post(&sem2);
    sem_post(&sem1);
    sem_post(&sem3);
}

void yzx() 
{
    int x;
	int y;
    int z;
	sem_wait(&sem2);
	y = Y;
	printf("Y = %d was put in th2\n", y);
	sleep(1);
	sem_wait(&sem3);
	z = Z;
    printf("Z = %d was put in th2\n", z);
    sem_wait(&sem1);
    x = X;
	printf("X = %d was put in th2\n", x);
	printf("X*Y*Z = %d\n", x * y * z);
	sem_post(&sem2);
    sem_post(&sem1);
    sem_post(&sem3);
}

void zxy() 
{
	int x;
	int y;
    int z;
	sem_wait(&sem3);
	z = Z;
	printf("Z = %d was put in th3\n", x);
	
	sem_wait(&sem1);
	x = X;
    printf("X = %d was put in th3\n", x);
    sem_wait(&sem2);
	y = Y;
	printf("Y = %d was put in th3\n", y);
	printf("X*Y*Z = %d\n", x * y * z);
	sem_post(&sem2);
    sem_post(&sem1);
    sem_post(&sem3);
}


int main() {
pthread_t thread1, thread2, thread3;

if (pthread_create(&thread1, NULL, (void *)xyz, NULL) != 0) 
{
	perror("pthread_create");
	exit(1);
}

if (pthread_create(&thread2, NULL, (void *)yzx, NULL) != 0)
{
	perror("pthread_create");
	exit(1);
}

if (pthread_create(&thread3, NULL, (void *)zxy, NULL) != 0)
{
	perror("pthread_create");
	exit(1);
}

if (pthread_join(thread1, NULL) != 0) 
{
	perror("pthread_join");
	exit(1);
}

if (pthread_join(thread2, NULL) != 0) 
{
	perror("pthread_join");
	exit(1);
}

if (pthread_join(thread3, NULL) != 0) 
{
	perror("pthread_join");
	exit(1);
}
  return 0;
}
