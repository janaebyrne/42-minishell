/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_redirection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbyrne <jbyrne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 21:31:55 by janaebyrne        #+#    #+#             */
/*   Updated: 2024/12/16 20:42:35 by jbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	open_file(const char *file, int flags, mode_t mode)
{
	int	fd;

	fd = open(file, flags, mode);
	if (fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

void	redirect_fd(int oldfd, int newfd)
{
	int	result;

	result = dup2(oldfd, newfd);
	if (result == -1)
	{
		perror("dup2");
		close(oldfd);
		exit(EXIT_FAILURE);
	}
	close(oldfd);
}

void	setup_redirection(t_cmd *node)
{
	int	fd;

	if (node->heredoc_delimiter)
	{
		fd = handle_heredoc(node->heredoc_delimiter, node->env_list);
		redirect_fd(fd, STDIN_FILENO);
	}
	else if (node->input_file)
	{
		fd = open_file(node->input_file, O_RDONLY, 0);
		redirect_fd(fd, STDIN_FILENO);
	}
	else if (node->output_file && node->append == 0)
	{
		fd = open_file(node->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		redirect_fd(fd, STDOUT_FILENO);
	}
	else if (node->append == 1)
	{
		fd = open_file(node->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		redirect_fd(fd, STDOUT_FILENO);
	}
}
