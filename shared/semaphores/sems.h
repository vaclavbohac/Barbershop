#ifndef SEMS_H
#define SEMS_H

// Semaphores definitions.
#define SEM_MUTEX      0
#define SEM_BARBER     1
#define SEM_CUSTOMMERS 2

// Number of semaphores.
#define SEM_COUNT     3

int semaphores_init(int key);

int down(int semid, int semaphore);

int up(int semid, int semaphore);

#endif
