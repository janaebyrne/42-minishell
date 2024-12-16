/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shkaruna <shkaruna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 22:37:17 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/15 22:37:25 by shkaruna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_echo(t_cmd *cmd_list)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (cmd_list->args[1] && ft_strcmp(cmd_list->args[1], "-n") == 0)
	{
		newline = 0;
		i = 2;
	}
	while (cmd_list->args[i])
	{
		printf("%s", cmd_list->args[i]);
		if (cmd_list->args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
