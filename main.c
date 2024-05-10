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


/* Defining a delimiter to use in strtok  */
#define DELIM "\t \n"


/*
 * _strlen - Get the length of a string
 * @str: string to get length
 * Return: number of chars in str (length)
 */

int _strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}



/*
 * _strdup - Duplicates a string (allocates memory)
 * @str: string to duplicate
 * Return: new string duplicated (allocated)
 * Or NULL if it fails to do so
 */



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


/*
 * _free - Frees a 2D arrays of strings
 * @cmd: array to free
 * Return: None return ,  void func
 */

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



/*
 * _getline - self  made of the getline func
 * reads a line from stream 
 * @buffer: Buffer to hold input
 * @nbuf: number of chars to be read
 * @stream: file-descriptor to read from (mostly STDIN)
 * Return: Number of chars readed 
 * or 0 if it reached EOF (end of line)
 */

size_t _getline(char **buffer, size_t *nbuf, FILE *stream)
{
	*buffer = malloc(sizeof(char) * 1000);
	stream = stdin;

	return (read(1, *buffer, 1000));
}


/*
 * split - splits a long line (command) into arrays of strings
 * Command arguments
 * @str: full command to split
 * Return: 2D array of strnigs or NULL if it failed
 */

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


/*
 * main - main function or / 
 * Entry point of the programm
 * @argc: number of args passed at execution 
 * @argv: 2D array of args
 * @envp: environnement vars in form of 2D arrays of strings
 * Return: 0 on success else an int
 */

int main(int ac, char **av, char **envp)
{
	;
	size_t MAX_BUF = 0;
	int i = 0, status;
	char **cmd = NULL, *buffer;
	size_t nchars;
	pid_t forked;
	while (1)
	{
		/* checking if its in interactive mode or not */
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "[dias@penguin]# ", 16);

		/* reading from STDIN_FILENO*/
		nchars = getline(&buffer, &MAX_BUF, stdin);
		if (nchars == -1)
		{
			free(buffer);
			exit(0);
		}

		/* split read command */
		cmd = split(buffer);
		if (!cmd || nchars == 1)
			continue;
		/* forking the next codeblock */
		forked = fork();
		if (forked == 0)
		{
			/* executing the command after split */
			if (execve(cmd[0], cmd, envp) == -1)
				_free(cmd);
		}

		else
		/* if its not the child process 
		 * it should wait to child to finish
		 */
			waitpid(forked, &status, 0);

		if (cmd != NULL)
			_free(cmd);
	}
	free(buffer);
	return (EXIT_SUCCESS);
}
