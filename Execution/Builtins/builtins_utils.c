/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shkaruna <shkaruna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 22:18:56 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/15 23:07:33 by shkaruna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_env_value(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

void	update_env_var(t_env **env_list, const char *key, const char *value)
{
	t_env	*current;
	t_env	*new_node;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	new_node = malloc(sizeof(t_env));
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = *env_list;
	*env_list = new_node;
}

// int	is_valid_identifier(const char *str)
// {
// 	int	i;

// 	if (!ft_isalpha(str[0]) && str[0] != '_')
// 		return (0);
// 	i = 1;
// 	while (str[i] != '\0' && str[i] != '=')
// 	{
// 		if (!ft_isalnum(str[i]) && str[i] != '_')
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

// void	set_value(char **dest, const char *src)
// {
// 	int	i;

// 	if (*dest)
// 		free(*dest);
// 	*dest = NULL;
// 	if (src)
// 	{
// 		*dest = malloc(ft_strlen(src) + 1);
// 		if (*dest)
// 		{
// 			i = 0;
// 			while (src[i])
// 			{
// 				(*dest)[i] = src[i];
// 				i++;
// 			}
// 			(*dest)[i] = '\0';
// 		}
// 	}
// }

// void	add_or_update_env(t_env **env_list, const char *key,
// 		const char *value)
// {
// 	t_env	*current;
// 	t_env	*new_node;

// 	current = *env_list;
// 	while (current)
// 	{
// 		if (ft_strcmp(current->key, key) == 0)
// 		{
// 			set_value(&current->value, value);
// 			return ;
// 		}
// 		current = current->next;
// 	}
// 	new_node = malloc(sizeof(t_env));
// 	if (!new_node)
// 		return ;
// 	new_node->key = ft_strdup(key);
// 	set_value(&new_node->value, value);
// 	new_node->next = *env_list;
// 	*env_list = new_node;
// }