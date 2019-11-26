#include <iostream>
#include <pthread.h>
using namespace std;

#define max 16

#define thread_max 4

int a[max] = { 1, 5, 7, 10, 12, 14, 15,
			18, 20, 22, 25, 27, 30,
			64, 110, 220 };


int f = 0;

int current_thread = 0;

void* ThreadSearch(void* args)
{
	int num = current_thread++;

	for (int i = num * (max / 4);
		i < ((num + 1) * (max / 4)); i++)
	{
		if (a[i] == key)
			f = 1;
	}
}

int main()
{
	pthread_t thread[thread_max];

	for (int i = 0; i < thread_max; i++) {
		pthread_create(&thread[i], NULL,
					ThreadSearch, (void*)NULL);
	}

	for (int i = 0; i < thread_max; i++) {
		pthread_join(thread[i], NULL);
	}

	if (f == 1)
		cout << "Key element found" << endl;
	else
		cout << "Key not present" << endl;
	return 0;
}
