/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_redirection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shkaruna <shkaruna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 21:31:55 by janaebyrne        #+#    #+#             */
/*   Updated: 2024/12/16 01:25:52 by shkaruna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"  

static char *expand_env(char *str, t_env *env_list)
{
    size_t result_size = 1; // Start with 1 for the null terminator
    char *start = str;
    char *end;

    // First pass: calculate the required size
    while ((end = strchr(start, '$')) != NULL)
    {
        result_size += end - start;

        // Find the end of the variable name
        start = end + 1;
        while (*start && (isalnum(*start) || *start == '_'))
            start++;

        // Get the variable name
        size_t len = start - end - 1;
        char *var_name = strndup(end + 1, len);

        // Find the variable value
        char *var_value = NULL;
        for (t_env *env = env_list; env; env = env->next)
        {
            if (strcmp(env->key, var_name) == 0)
            {
                var_value = env->value;
                break;
            }
        }

        // Add the length of the variable value
        if (var_value)
            result_size += strlen(var_value);

        free(var_name);
    }
    result_size += strlen(start);

    // Allocate memory for the result
    char *result = malloc(result_size);
    if (!result)
        return NULL;

    result[0] = '\0';
    start = str;

    // Second pass: build the result string
    while ((end = strchr(start, '$')) != NULL)
    {
        // Append the part before the $
        strncat(result, start, end - start);

        // Find the end of the variable name
        start = end + 1;
        while (*start && (isalnum(*start) || *start == '_'))
            start++;

        // Get the variable name
        size_t len = start - end - 1;
        char *var_name = strndup(end + 1, len);

        // Find the variable value
        char *var_value = NULL;
        for (t_env *env = env_list; env; env = env->next)
        {
            if (strcmp(env->key, var_name) == 0)
            {
                var_value = env->value;
                break;
            }
        }

        // Append the variable value
        if (var_value)
            strcat(result, var_value);

        free(var_name);
    }

    // Append the rest of the string
    strcat(result, start);

    return result;
}
static char *strip_newline(char *str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0';  // Remove the newline
    }
    return str;
}


int open_file(const char *file, int flags, mode_t mode)
{
    int fd;
    fd = open(file, flags, mode);
    if (fd < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    return fd;
}
void redirect_fd(int oldfd, int newfd)
{
    int result;
    result = dup2(oldfd, newfd);
    if (result == -1)
    {
        perror("dup2");
        close(oldfd);
        exit(EXIT_FAILURE);
    }
    close(oldfd);
}

void setup_redirection(t_cmd *node)
{
    int fd;
    if (node->heredoc_delimiter)
    {
        fd = handle_heredoc(node->heredoc_delimiter, node->env_list);
        redirect_fd(fd, STDIN_FILENO);
    }
    else if (node->input_file)
    {
        // Redirect input from file (`<`)
        fd = open_file(node->input_file, O_RDONLY, 0);
        redirect_fd(fd, STDIN_FILENO);
    } 
    else if (node->output_file && node->append == 0)
    {
        // Redirect output to file, overwriting (`>`)
        fd = open_file(node->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        redirect_fd(fd, STDOUT_FILENO);
    } 
    else if (node->append == 1)
    {
        // Redirect output to file, appending (`>>`)
        fd = open_file(node->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        redirect_fd(fd, STDOUT_FILENO);
    } 
}

int handle_heredoc(char *delimiter, t_env *env_list)
{
    char *line = NULL;
    char *expanded_line = NULL;
    int fd;
    ssize_t bytes_written;

    strip_newline(delimiter);

    // Open the temporary heredoc file for writing
    fd = open_file("heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("Error opening heredoc file");
        exit(EXIT_FAILURE);
    }

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

        // Check if the line matches the delimiter
        if (strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }

        // Expand environment variables in the input line
        expanded_line = expand_env(line, env_list);
        free(line);

        // Write the expanded line to the heredoc file
        bytes_written = write(fd, expanded_line, ft_strlen(expanded_line));
        free(expanded_line);

        if (bytes_written == -1)
        {
            perror("Error writing to heredoc file");
            close(fd);
            exit(EXIT_FAILURE);
        }

        // Add a newline to the heredoc file
        write(fd, "\n", 1);
    }

    close(fd);

    // Reopen the heredoc file for reading
    fd = open_file("heredoc", O_RDONLY, 0);
    if (fd < 0)
    {
        perror("Error opening heredoc file for reading");
        exit(EXIT_FAILURE);
    }

    return fd;
}
