/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grmullin <grmullin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 12:12:56 by grmullin          #+#    #+#             */
/*   Updated: 2024/10/10 12:12:57 by grmullin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	execute_builtin(t_cmd *cmd_list, t_env *env_list);

int	ft_built_ins(t_cmd *cmd_list, t_env *env_list)
{
	int	stdin_copy;
	int	stdout_copy;
	int	status;

	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	if (stdin_copy < 0 || stdout_copy < 0)
	{
		perror("dup failed");
		return (-1);
	}
	setup_redirection(cmd_list);
	status = execute_builtin(cmd_list, env_list);
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
	return (status);
}

static int	execute_builtin(t_cmd *cmd_list, t_env *env_list)
{
	int	status;

	status = 0;
	if (ft_strcmp(cmd_list->command, "echo") == 0)
		status = (ft_echo(cmd_list));
	else if (ft_strcmp(cmd_list->command, "pwd") == 0)
		status = (ft_pwd(cmd_list));
	else if (ft_strcmp(cmd_list->command, "cd") == 0)
		status = (ft_cd(&env_list, cmd_list->args[1]));
	else if (ft_strcmp(cmd_list->command, "env") == 0)
		status = (ft_env(cmd_list, env_list));
	else if (ft_strcmp(cmd_list->command, "unset") == 0)
		status = (ft_unset(cmd_list, &env_list));
	else if (ft_strcmp(cmd_list->command, "export") == 0)
		status = (ft_export(cmd_list, &env_list));
	else if (ft_strcmp(cmd_list->command, "exit") == 0)
	{
		ft_exit(cmd_list);
		status = 0;
	}
	return (status);
}
