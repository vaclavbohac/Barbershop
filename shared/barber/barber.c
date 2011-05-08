#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "barber.h"

void cut_hair(int time)
{
	printf("Barber: Cutting hair for %d seconds.\n", time);
	sleep(time);
	printf("Barber: Hair cutted.\n");
}
