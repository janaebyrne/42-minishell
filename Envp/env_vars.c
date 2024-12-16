/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shkaruna <shkaruna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 21:42:42 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/15 22:03:22 by shkaruna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_list(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

void	add_to_list(t_env **env_list, char *key, char *value)
{
	t_env	*new_node;
	t_env	*current;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("Failed to allocate memory!\n");
		return ;
	}
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	if (*env_list == NULL)
	{
		*env_list = new_node;
	}
	else
	{
		current = *env_list;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
}

t_env	*env_init(char **envp)
{
	t_env	*env_list;
	char	*key;
	char	*value;
	char	*env_copy;
	char	*equal_sign;

	env_list = NULL;
	while (*envp)
	{
		env_copy = ft_strdup(*envp);
		if (!env_copy)
			return (NULL);
		equal_sign = ft_strchr(env_copy, '=');
		if (equal_sign != NULL)
		{
			*equal_sign = '\0';
			key = env_copy;
			value = equal_sign + 1;
			add_to_list(&env_list, key, value);
		}
		free(env_copy);
		envp++;
	}
	return (env_list);
}

char	*get_env_var(t_env *env_list, char *key)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (0);
}
