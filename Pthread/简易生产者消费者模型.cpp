#include <stdlib.h>
#include <stdio.h>
#include "pthread.h"
#include "unistd.h"


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <windows.h>
#include <time.h>
#include <stdint.h>
#include <semaphore.h>
#pragma comment(lib,"x86/pthreadVC2.lib")



struct msg {
	struct msg *next;
	int num;
};

struct msg *head;
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *p)
{
	struct msg *mp;
	while (1) {
		mp = malloc(sizeof(struct msg));
		mp->num = rand() % 1000 + 1;
		printf("produce %d\n", mp->num);

		pthread_mutex_lock(&lock);
		mp->next = head;
		head = mp;
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&has_product);
		sleep(2);
	}
}

void *consumer(void *p)
{
	while (1) {
		pthread_cond_wait(&has_product, &lock);

		struct msg *tmp = head;
		while (tmp != NULL) {
			printf("%d ", tmp->num);
			tmp = tmp->next;
		}
		printf("\n");
	}
}


int main()
{
	pthread_t pid, cid;
	pthread_create(&pid, NULL, producer, NULL);
	pthread_create(&cid, NULL, consumer, NULL);
	pthread_join(pid, NULL);
	pthread_join(cid, NULL);

	return 0;
}


/*

线程间同步的一种情况：线程A需要等某个条件成立才能继续往下执行，现在这个条件不成立，线程A就阻塞等待，而线程B在执行过程中使这个条件成立了，就唤醒线程A继续执行。
在pthread库中通过条件变量(Condition Variable)来阻塞等待一个条件，或者唤醒等待这个条件的线程 。
一个生产者-消费者的例子：消费者A等待生产者B产出产品后才打印，否则消费者阻塞等待生产者生产。

*/