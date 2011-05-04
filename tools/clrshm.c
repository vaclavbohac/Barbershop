#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "shmemory/shared.h"

int main (void) {
	int shmid, mask = 0666, uid = getuid();

	shmid = shmget(uid, sizeof(struct shared*), mask);

	if (shmid == -1) {
		fprintf(stderr, "There is no shared memory.\n");
		exit(EXIT_FAILURE);
	}

	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
		perror("Shared memory could not be deleted");
		exit(EXIT_FAILURE);
	}

	printf("Shared memory was deleted.\n");
	return EXIT_SUCCESS;
}
