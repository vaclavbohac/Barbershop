#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "sems.h"

int down(int semid, int semaphore)
{
	struct sembuf operation[1];
	operation[0].sem_num = semaphore;
	operation[0].sem_op = -1;
	operation[0].sem_flg = 0;
	return semop(semid, operation, 1);
}

int up(int semid, int semaphore)
{
	struct sembuf operation[1];
	operation[0].sem_num = semaphore;
	operation[0].sem_op = 1;
	operation[0].sem_flg = 0;
	return semop(semid, operation, 1);
}

int semaphores_init(int key)
{
	int semaphores = semget(key, SEM_COUNT, 0666);
	if (semaphores == -1) {
		semaphores = semget(key, SEM_COUNT, 0666 | IPC_CREAT);
		if (semaphores == -1) {
			return -1;
		}
		
		// Set up  mutual exclusion.
		if (semctl(semaphores, SEM_MUTEX, SETVAL, 1) == -1) {
			return -1;
		}

		// Set up customers.
		if (semctl(semaphores, SEM_CUSTOMMERS, SETVAL, 0) == -1) {
			return -1;
		}

		// Set up barber.
		if (semctl(semaphores, SEM_BARBER, SETVAL, 0) == -1) {
			return -1;
		}

		// Set up haircut.
		if (semctl(semaphores, SEM_HAIRCUT, SETVAL, 0) == -1) {
			return -1;
		}
	}
	return semaphores;
}
