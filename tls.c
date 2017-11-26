#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/mman.h>

#define MAX_THREADS 256
#define TRUE 1
#define FALSE 0
int tls_count = 0;
pthread_t current_thread_id;
int page_size = getpagesize();
struct page
{
	unsigned int address;
	int share_count;
};

struct tls_storage
{
	pthread_t thread_id;
	unsigned int tls_size;
	unsigned int num_pages;
	int destroyed = TRUE;

};

struct tls_storage tls_storage_array[MAX_THREADS];

int find_tls(pthread_t look_up_id){
	for (int i = 0; i < tls_count; ++i)
	{
		if (tls_storage_array[i].thread_id == look_up_id)
		{
			if (tls_storage_array[i].destroyed)
			{
				return 0;
			}
			return i; // returns the tls_storage_array index if found
		}
	}
	return 0;	//returns 0 if no tls found
}

int tls_create(unsigned int size){
	if (size < 0)
	{
		return -1; //Error
	}
	current_thread_id = pthread_self();

	if (find_tls(current_thread_id) != 0)
	{
		return -1; //Error TLS exists
	}
	if (tls_count == 0)
	{
		//initialize the SIGSEGV handler
		
	}

	tls_storage_array[tls_count].thread_id = current_thread_id;
	tls_storage_array[tls_count].tls_size = size;
	tls_storage_array[tls_count].destroyed = FALSE;
	tls_count++;
	return 0;
}

int tls_write(unsigned int offset, unsigned int length, char
*buffer){
	return 0;
}
int tls_destroy(){
	find_tls(pthread_self())
}