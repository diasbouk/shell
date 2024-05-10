#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct stat st;
extern char **environ;

int _strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}

char *_strstr(char *src, char *find)
{
	int i = 0, j = 0;

	while (find[j] && src[i])
	{
		if (src[i + j] == find[j])
			j++;
		else
		{
			j = 0;
			i++;
		}
	}

	if (*(src + i))
		return (src + i);
	else
		return (NULL);
}

char *_getenv(char *name, char **env)
{
	int i = 0;
	while (env[i])
	{
		if (strncmp(env[i], name, _strlen(name)) == 0)
			return ((_strstr(env[i], "=")) + 1);
		i++;
	}
	return (NULL);
}

char *path() {}

char *single_path(char *path)
{
	int i = 0;
	char *token = NULL;

	while (path[i])
	{
		if (path[i] == ':')
			break;
		else
			i++;
	}

	token = (char *)malloc(sizeof(char) * (i + 1));
	i = 0;

	while (path[i])
	{
		if (path[i] == ':')
			break;
		else
		{
			token[i] = path[i];
			i++;
		}
	}
	return (token);
}

char *get_command(char *word)
{
	char *token, *full_cmd = NULL;
	char *path = _getenv("PATH", environ);
	token = strtok(path, ":");
	while (token)
	{
		full_cmd = (char *)realloc(
			full_cmd, sizeof(char) * (strlen(token) + strlen(word) + 1));
		strcpy(full_cmd, token);
		strcat(full_cmd, "/");
		strcat(full_cmd, word);
		if (stat(full_cmd, &st) == 0)
			return (full_cmd);

		else
			token = strtok(NULL, ":");
	}
	return (full_cmd);
}

int main(int argc, char *argv[], char **envp)
{
	(void)argc;
	(void)argv;
	char *buffer = NULL, *cmd;
	size_t MAX_BUF, nchars;

	while (1)
	{
		nchars = getline(&buffer, &MAX_BUF, stdin);
		if (nchars == -1)
		{
			free(buffer);
			exit(1);
		}

		cmd = get_command(buffer);
		printf("%s\n", cmd);
	}

	return 0;
}
