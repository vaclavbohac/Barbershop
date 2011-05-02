#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "shared.h"


struct shared* get_shared(int key)
{
	int shmid, mask = 0666;
	shmid = shmget(key, sizeof(struct shared*), mask);
	if (shmid == -1) {
		shmid = shmget(key, sizeof(struct shared*), mask | IPC_CREAT);
		if (shmid == -1) {
			perror("Shared memory could not be attached");
			return NULL; 
		}
	}
	struct shared* data = (struct shared*) shmat(shmid, NULL, 0);
	if (data == (void*) -1) {
		perror("Memory could not be attached.");
		return NULL;
	}
	return data;
}
