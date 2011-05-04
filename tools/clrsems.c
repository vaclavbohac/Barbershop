#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "semaphores/sems.h"

int main( void )
{
	int semaphores, uid = getuid();
	semaphores = semaphores_init(uid);	

	if (semctl(semaphores, 0, IPC_RMID, NULL) == -1) {
		perror("Semaphores could be removed");
		return EXIT_FAILURE;
	}

	printf("Semaphores canceled.\n");
	return EXIT_SUCCESS;
}
