/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shkaruna <shkaruna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:12:33 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/16 00:56:33 by shkaruna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_setup_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	return (0);
}

static void	ft_child_process(t_cmd *cmd, int prev_fd, int pipe_fd[2],
		t_shell *shell)
{
	if (prev_fd != -1 && (dup2(prev_fd, STDIN_FILENO) == -1 || close(prev_fd) ==
			-1))
	{
		perror("dup2 input from pipe");
		exit(EXIT_FAILURE);
	}
	if (cmd->next && (dup2(pipe_fd[1], STDOUT_FILENO) == -1
			|| close(pipe_fd[1]) == -1))
	{
		perror("dup2 output to pipe");
		exit(EXIT_FAILURE);
	}
	if (cmd->next && close(pipe_fd[0]) == -1)
	{
		perror("close pipe read end");
		exit(EXIT_FAILURE);
	}
	if (ft_execute(cmd, shell) == -1)
	{
		perror("Execution failed");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

static int	ft_parent_process(pid_t pid, int *prev_fd, int pipe_fd[2],
		t_cmd *cmd)
{
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (cmd->next)
		close(pipe_fd[1]);
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
		*prev_fd = pipe_fd[0];
	return (0);
}

int	ft_execute_pipeline(t_cmd *cmd, t_shell *shell)
{
	int		pipe_fd[2];
	int		prev_fd;
	pid_t	pid;

	prev_fd = -1;
	while (cmd)
	{
		if (cmd->next && ft_setup_pipe(pipe_fd) == -1)
			return (-1);
		if (cmd->command == NULL || cmd->command[0] == '\0')
		{
			fprintf(stderr, "Syntax error: unexpected token '||'\n");
			return (-1);
		}
		pid = fork();
		if (pid == 0)
			ft_child_process(cmd, prev_fd, pipe_fd, shell);
		if (ft_parent_process(pid, &prev_fd, pipe_fd, cmd) == -1)
			return (-1);
		cmd = cmd->next;
	}
	while (wait(NULL) > 0)
		;
	return (0);
}
