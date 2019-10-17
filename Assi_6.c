//33158

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#define NO 10

enum 
{
	THINKING,
	EATING,
	HUNGRY
}state[NO];

sem_t s[NO];
pthread_mutex_t mutex;

void *philosopher(void *arg);
void test(int i);
void take_fork(int i);
void put_fork(int i);

int main()
{
	pthread_t *p;
	int *index;
	
	for(int i=0;i<NO;++i)
	{
		state[i] = THINKING;
		sem_init(&s[i],0,0);
	}
	
	pthread_mutex_init(&mutex,NULL);
	
	p = (pthread_t *)malloc(NO * sizeof(pthread_t));
	index = (int*)malloc(NO * sizeof(int));
	
	for(int i=0;i<NO;++i)
	{
		index[i] = i;
		pthread_create(&p[i],NULL,philosopher,(void*)&index[i]);
	}
	
	for(int i=0;i<NO;++i)
	{
		pthread_join(p[i],NULL);
	}
}

void *philosopher(void *arg)
{
	int i = *(int*)arg;
	
	while(1)
	{
		printf("\n\nPHILOSOPHER %d :: THINKING!!",i+1);
		sleep(2);
		
		take_fork(i);
		
		printf("\n\n\t\tPHILOSOPHER %d :: EATING!!",i+1);
		sleep(2);
		
		put_fork(i);
	}
}

void test(int i)
{
	if((state[i]==HUNGRY) && 
		(state[(i+1)%NO]!=EATING) && 
		(state[(i+NO-1)%NO]!=EATING))
	{
		state[i] = EATING;
		sem_post(&s[i]);
	}
}

void take_fork(int i)
{
	pthread_mutex_lock(&mutex);
	printf("\n\nPHILOSOPHER %d :: Hungry!!",i+1);
	state[i] = HUNGRY;
	test(i);
	pthread_mutex_unlock(&mutex);
	if(state[i]!=EATING)
		sem_wait(&s[i]);
}

void put_fork(int i)
{
	pthread_mutex_lock(&mutex);
	state[i] = THINKING;
	
	printf("\n\nPHILOSOPHER %d :: Finished eating!!",i+1);
	
	test((i+1)%NO);
	test((i+NO-1)%NO);
	pthread_mutex_unlock(&mutex);
}
