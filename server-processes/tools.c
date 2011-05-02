#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "help.h"

void rtrim(char* str)
{
	int i = strlen(str) - 1;
	while (i >= 0 && isspace(str[i])) {
		i -= 1;
	}
	str[i + 1] = '\0';
}
