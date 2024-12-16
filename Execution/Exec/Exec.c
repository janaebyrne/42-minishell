/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shkaruna <shkaruna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 17:13:01 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/15 23:32:12 by shkaruna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_is_builtin(char *command)
{
	if (ft_strncmp(command, "echo ", 5) == 0 || (ft_strncmp(command, "echo",
				4) == 0 && ft_strlen(command) == 4) || ft_strncmp(command,
			"cd ", 3) == 0 || (ft_strncmp(command, "cd", 2) == 0
			&& ft_strlen(command) == 2) || ft_strncmp(command, "pwd ", 4) == 0
		|| (ft_strncmp(command, "pwd", 3) == 0 && ft_strlen(command) == 3)
		|| ft_strncmp(command, "export ", 7) == 0 || (ft_strncmp(command,
				"export", 6) == 0 && ft_strlen(command) == 6)
		|| ft_strncmp(command, "unset ", 6) == 0 || (ft_strncmp(command,
				"unset", 5) == 0 && ft_strlen(command) == 5)
		|| ft_strncmp(command, "env ", 4) == 0 || (ft_strncmp(command, "env",
				3) == 0 && ft_strlen(command) == 3) || (ft_strncmp(command,
				"exit", 4) == 0 && ft_strlen(command) == 4))
		return (1);
	return (0);
}

int	ft_execute_main(t_cmd *cmd_list, t_shell *shell)
{
	int	status;

	if (!cmd_list->next)
	{
		if (ft_is_builtin(cmd_list->command))
		{
			status = ft_built_ins(cmd_list, shell->env_list);
			shell->last_exit_status = status;
			return (status);
		}
		status = ft_execute_external_command(cmd_list, shell->env_list);
		shell->last_exit_status = status;
		return (status);
	}
	status = ft_execute_pipeline(cmd_list, shell);
	shell->last_exit_status = status;
	return (status);
}

int	ft_execute(t_cmd *cmd_list, t_shell *shell)
{
	int	status;

	if (ft_is_builtin(cmd_list->command))
	{
		status = ft_built_ins(cmd_list, shell->env_list);
		shell->last_exit_status = status;
		return (status);
	}
	status = ft_execute_external_command(cmd_list, shell->env_list);
	shell->last_exit_status = status;
	return (status);
}
