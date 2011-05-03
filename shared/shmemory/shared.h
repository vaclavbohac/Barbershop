#ifndef SHARED_H
#define SHARED_H

struct shared {
	int custommers;
	int semaphores;
	int times[5];
	int handles[5];
};


struct shared* get_shared(int key);

#endif
