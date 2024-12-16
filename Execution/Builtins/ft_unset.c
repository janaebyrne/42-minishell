/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shkaruna <shkaruna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 10:42:04 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/15 23:26:25 by shkaruna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	remove_from_export_list(t_env **export_list, const char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *export_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*export_list = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

static int	is_valid_identifier_unset(const char *str)
{
	int	i;

	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] != '\0')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	remove_env_var(t_env **env_list, const char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env_list = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	ft_unset(t_cmd *cmd_list, t_env **env_list)
{
	static t_env	*export_list;
	int				i;

	export_list = NULL;
	if (!cmd_list->args[1])
		return (0);
	i = 1;
	while (cmd_list->args[i])
	{
		if (is_valid_identifier_unset(cmd_list->args[i]))
		{
			remove_env_var(env_list, cmd_list->args[i]);
			remove_from_export_list(&export_list, cmd_list->args[i]);
		}
		else
		{
			printf("unset: `%s': not a valid identifier\n", cmd_list->args[i]);
		}
		i++;
	}
	return (0);
}
