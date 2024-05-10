#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define DELIM "\t \n"


int _strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}


char *_strdup(char *str)
{
	int i;
	char *token = (char *)malloc(sizeof(char) * (_strlen(str) + 1));
	if (token == NULL)
		return NULL;
	for (i = 0; str[i]; i++)
		token[i] = str[i];
	return (token);
};



int main()
{
	char str[] = "saidExact";
	char *dup = strdup(str);
	printf("%s\n", dup);
	free(dup);
	return 0;
}
