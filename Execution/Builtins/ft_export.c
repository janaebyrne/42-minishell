/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbyrne <jbyrne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:40:01 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/16 20:35:07 by jbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_valid_identifier(const char *str)
{
	int	i;

	i = 1;
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	while (str[i] != '\0' && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	handle_argument(t_env **env_list, const char *arg)
{
	char	*key;
	char	*value;

	key = ft_strdup(arg);
	value = ft_strchr(key, '=');
	if (value)
	{
		*value = '\0';
		value++;
		if (!is_valid_identifier(key))
		{
			printf("export: `%s': not a valid identifier\n", arg);
			free(key);
			return ;
		}
		add_or_update_env(env_list, key, value);
	}
	else
	{
		if (!is_valid_identifier(key))
			printf("export: `%s': not a valid identifier\n", key);
		else
			add_or_update_env(env_list, key, NULL);
	}
	free(key);
}

t_env	*copy_env_list(t_env *env_list)
{
	t_env	*new_list;
	t_env	*current;
	t_env	*new_node;

	new_list = NULL;
	current = env_list;
	while (current)
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		new_node->key = ft_strdup(current->key);
		if (current->value)
			new_node->value = ft_strdup(current->value);
		else
			new_node->value = NULL;
		new_node->next = new_list;
		new_list = new_node;
		current = current->next;
	}
	return (new_list);
}

int	ft_export(t_cmd *cmd_list, t_env **env_list)
{
	int	i;

	if (!cmd_list->args[1])
	{
		print_sorted_env_list(*env_list);
		return (0);
	}
	i = 1;
	while (cmd_list->args[i])
	{
		handle_argument(env_list, cmd_list->args[i]);
		i++;
	}
	return (0);
}
