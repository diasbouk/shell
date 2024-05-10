#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define DELIM "\t "

char **split(char *str)
{
	char *token, *str_clone;
	int i = 1, j = 0;
	char **full_cmd = NULL;
	str_clone = strdup(str);
	token = strtok(str_clone,  DELIM);

	while (token)
	{
		token = strtok(NULL, DELIM);
		i++;
	}
	free(str_clone);

	full_cmd = (char **)malloc(sizeof(char *) * i + 1);
	token = strtok(str,  DELIM);
 
	while (token)
	{
		full_cmd[j] = (char *)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(full_cmd[j], token);
		j++;
		token = strtok(NULL, DELIM);
	}
	return (full_cmd);
}

int main(int argc, char *argv[])
{
	int i = 0;

	char str[] = "some more stuuff\t down here nman \n";
	char **cmd = split(str);
	while (cmd[i])
	{
		printf("%s\n", cmd[i]);
		i++;
	}
	for (int j = 0; j < i; j++)
	{
		free(cmd[j]);
	}
	free(cmd);
	return 0;
}
