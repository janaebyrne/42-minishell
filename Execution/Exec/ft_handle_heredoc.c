/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbyrne <jbyrne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:13:23 by jbyrne            #+#    #+#             */
/*   Updated: 2024/12/16 18:14:29 by jbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*strip_newline(char *str)
{
	size_t	len;

	len = strlen(str);
	if (len > 0 && str[len - 1] == '\n')
	{
		str[len - 1] = '\0';
	}
	return (str);
}

static int open_heredoc_file_for_writing()
{
    int fd = open_file("heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("Error opening heredoc file");
        exit(EXIT_FAILURE);
    }
    return fd;
}

static void process_heredoc_input(int fd, char *delimiter, t_env *env_list)
{
    char *line = NULL;
    char *expanded_line = NULL;
    ssize_t bytes_written;

    while (1)
    {
        write(STDOUT_FILENO, "> ", 2);
        line = get_next_line(STDIN_FILENO);
        if (line == NULL)
        {
            perror("Error reading from stdin");
            close(fd);
            exit(EXIT_FAILURE);
        }
        strip_newline(line);
        if (strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        expanded_line = expand_env(line, env_list);
        free(line);
        bytes_written = write(fd, expanded_line, ft_strlen(expanded_line));
        free(expanded_line);
        if (bytes_written == -1)
        {
            perror("Error writing to heredoc file");
            close(fd);
            exit(EXIT_FAILURE);
        }
        write(fd, "\n", 1);
    }
}

static int open_heredoc_file_for_reading()
{
    int fd = open_file("heredoc", O_RDONLY, 0);
    if (fd < 0)
    {
        perror("Error opening heredoc file for reading");
        exit(EXIT_FAILURE);
    }
    return fd;
}

int handle_heredoc(char *delimiter, t_env *env_list)
{
    strip_newline(delimiter);
    int fd = open_heredoc_file_for_writing();
    process_heredoc_input(fd, delimiter, env_list);
    close(fd);
    return open_heredoc_file_for_reading();
}