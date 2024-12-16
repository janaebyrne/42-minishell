/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_update_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbyrne <jbyrne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 20:29:42 by jbyrne            #+#    #+#             */
/*   Updated: 2024/12/16 20:32:31 by jbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	update_value(t_env *current, const char *value)
{
	free(current->value);
	if (value)
		current->value = ft_strdup(value);
	else
		current->value = NULL;
}

static void	add_new_node(t_env **env_list, const char *key, const char *value)
{
	t_env	*new_node;
	t_env	*current;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	if (!*env_list)
		*env_list = new_node;
	else
	{
		current = *env_list;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

void	add_or_update_env(t_env **env_list, const char *key,
		const char *value)
{
	t_env	*current;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			update_value(current, value);
			return ;
		}
		current = current->next;
	}
	add_new_node(env_list, key, value);
}
