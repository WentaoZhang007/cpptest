#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
using namespace std;
pthread_mutex_t taxiMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t taxiCond = PTHREAD_COND_INITIALIZER;
void * traveler_arrive(void * name) {
	cout << "Traveler: " << (char*)name << " needs a taxi now " << endl;
	pthread_mutex_lock(&taxiMutex);
	pthread_cond_wait(&taxiCond, &taxiMutex);
	pthread_mutex_unlock(&taxiMutex);
	cout << "Traveler: " << (char*)name << " now got a taxi." << endl;
	pthread_exit((void*)0);
}
 
void * taxi_arrive(void * name) {
	cout << " Taxi " << (char*)name << " arrives." << endl;
	pthread_cond_signal(&taxiCond);
	pthread_exit((void*)0);
}
 
int main() 
{	
	pthread_t thread;
	pthread_attr_t threadAttr;
	pthread_attr_init(&threadAttr);
 
	pthread_create(&thread, &threadAttr, taxi_arrive, (void*)(" Jack "));
	sleep(1);
	pthread_create(&thread, &threadAttr, traveler_arrive, (void*)(" Susan "));
	sleep(1);
	pthread_create(&thread, &threadAttr, taxi_arrive, (void*)(" Mike "));
	sleep(1);
	return 0;
}

