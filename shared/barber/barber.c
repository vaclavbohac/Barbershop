#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "semaphores/sems.h"
#include "shmemory/shared.h"

#include "barber.h"

void cut_hair(int time)
{
	printf("Barber: Cutting hair for %d seconds.\n", time);
	sleep(time);
	printf("Barber: Hair cutted.\n");
}

void barber(void)
{
	// Get user id.
	int uid = getuid();

	// Set shared memory and semaphores.
	struct shared* data = get_shared(uid);
	data->semaphores = semaphores_init(uid);
	if (data->semaphores == -1) {
		perror("Semaphores initialization error");
		return;
	}
	// Set customers to the default state.
	data->custommers = 0;
	while (1) {
#ifdef DEBUG
		printf("Barber: Custommers down.\n");
#endif
		// Wait for custommer.
		down(data->semaphores, SEM_CUSTOMMERS);
		// Enter critical section.
		down(data->semaphores, SEM_MUTEX);
		// Decrease number of custommers.
		data->custommers -= 1;
		// Get time from custommer.
		int time = data->times[data->custommers];
		// Release the kraken.
		up(data->semaphores, SEM_BARBER);
		// Leave critical section.
		up(data->semaphores, SEM_MUTEX);

		// Cut custommer's hair.
		cut_hair(time);

		// Run closing procedure.
		up(data->semaphores, SEM_CUTTED);
		down(data->semaphores, SEM_SITTING);
	}
}
