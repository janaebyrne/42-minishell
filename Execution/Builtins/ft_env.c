/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shkaruna <shkaruna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 09:43:11 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/15 22:37:50 by shkaruna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	print_env_list(t_env *env_list)
{
	while (env_list)
	{
		if (env_list->value)
			printf("%s=%s\n", env_list->key, env_list->value);
		env_list = env_list->next;
	}
}

int	ft_env(t_cmd *cmd_list, t_env *env_list)
{
	(void)cmd_list;
	if (!env_list)
		return (1);
	else
	{
		print_env_list(env_list);
		return (0);
	}
}
