#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "pthread.h"

pthread_mutex_t lock;
pthread_cond_t  cond;

int sum;

void* dec(void *argv)
{
	pthread_mutex_lock(&lock);
	while (sum == 0)
	{
		printf("dec sum=0 ");
		pthread_cond_wait(&cond, &lock);  //???
	}
	sum--;
	pthread_mutex_unlock(&lock);
	printf("dec sum=%d ", sum);
	pthread_exit(NULL);
	return 0;
}

void* add(void *argv)
{
	pthread_mutex_lock(&lock);
	if (sum == 0)
	{
		printf("add sum =0 ");
		pthread_cond_signal(&cond);//???
	}
	sum++;

	pthread_mutex_unlock(&lock);
	sleep(1);
	sum++;
	printf("add sum=%d ", sum);
	pthread_exit(NULL);
	return 0;
}

int main(int argc, char** argv)
{
	pthread_t ptid1, ptid2;
	sum = 0;
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);

	pthread_create(&ptid1, NULL, dec, NULL);
	pthread_create(&ptid2, NULL, add, NULL);


	int counter = 0;
	while (counter != 10) {
		printf("counter: %d ", counter);
		sleep(1);
		counter++;
	}


	return 0;
}
