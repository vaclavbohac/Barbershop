#ifndef SHARED_H
#define SHARED_H

struct shared {
	int custommers;
	int semaphores;
};


struct shared* get_shared(int key);


#endif
