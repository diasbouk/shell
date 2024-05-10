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
	char *token = malloc(sizeof(char) * (_strlen(str) + 1));
	if (token == NULL)
		return NULL;
	for (i = 0; str[i]; i++)
		token[i] = str[i];
	token[i] = '\0';
	return (token);
};

void _free(char **cmd)
{
	int i = 0;

	if (cmd == NULL)
		return;
	for (i = 0; cmd[i]; i++)
	{
		free(cmd[i]);
		cmd[i] = NULL;
	}
	free(cmd);
	cmd = NULL;
}

size_t _getline(char **buffer, size_t *nbuf, FILE *stream)
{
	*buffer = malloc(sizeof(char) * 1000);
	stream = stdin;

	return (read(1, *buffer, 1000));
}

char **split(char *str)
{
	char *token, *str_clone;
	int i = 1, j = 0;
	char **full_cmd = NULL;
	str_clone = _strdup(str);
	token = strtok(str_clone, DELIM);
	if (!token)
	{
		free(str_clone);
		return (NULL);
	}

	while (token)
	{
		token = strtok(NULL, DELIM);
		i++;
	}
	free(str_clone);

	full_cmd = malloc(sizeof(char *) * (i + 1));
	token = strtok(str, DELIM);

	if (!token)
	{
		free(full_cmd);
		return (NULL);
	}

	while (token)
	{
		full_cmd[j] = _strdup(token);
		token = strtok(NULL, DELIM);
		j++;
	}
	full_cmd[j] = NULL;
	return (full_cmd);
}

int main(int ac, char **av, char **envp)
{
	char *buffer = NULL, *argv[] = {"/bin/ls", "-l", "/usr/", NULL};
	;
	size_t MAX_BUF = 0;
	int i = 0, status;
	char **cmd = NULL;
	size_t nchars;
	pid_t forked;
	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "[dias@penguin]# ", 16);

		nchars = getline(&buffer, &MAX_BUF, stdin);
		if (nchars == -1)
		{
			free(buffer);
			exit(0);
		}

		cmd = split(buffer);
		if (!cmd || nchars == 1)
			continue;
		forked = fork();
		if (forked == 0)
		{
			if (execve(cmd[0], cmd, envp) == -1)
				_free(cmd);
		}

		else
			waitpid(forked, &status, 0);

		if (cmd != NULL)
			_free(cmd);
	}
	free(buffer);
	return (0);
}
