/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shkaruna <shkaruna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:44:27 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/15 22:40:00 by shkaruna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_is_numeric(const char *str)
{
	if (!str || !*str)
		return (0);
	while (*str)
	{
		if (!ft_is_digit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	ft_exit(t_cmd *cmd)
{
	int	exit_status;

	exit_status = 0;
	if (cmd->args[0] && ft_strcmp(cmd->args[0], "exit") == 0)
	{
		printf("exit\n");
		if (cmd->args[1] == NULL)
			exit_status = 0;
		else if (cmd->args[2] != NULL)
			fprintf(stderr, "minishell: exit: too many arguments\n");
		else if (!ft_is_numeric(cmd->args[1]))
		{
			fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
				cmd->args[1]);
			exit_status = 255;
		}
		else
			exit_status = ft_atoi(cmd->args[1]);
		exit(exit_status);
	}
	return (0);
}
