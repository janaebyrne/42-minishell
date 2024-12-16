/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exernal_commands_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbyrne <jbyrne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 20:39:42 by jbyrne            #+#    #+#             */
/*   Updated: 2024/12/16 20:41:31 by jbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	count_env_list(t_env *env_list)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

char	*create_env_entry(t_env *env)
{
	int		len;
	char	*entry;

	len = ft_strlen(env->key) + ft_strlen(env->value) + 2;
	entry = malloc(len);
	if (!entry)
		return (NULL);
	sprintf(entry, "%s=%s", env->key, env->value);
	return (entry);
}

char	**env_list_to_array(t_env *env_list)
{
	int		count;
	t_env	*current;
	char	**envp;
	int		i;

	count = count_env_list(env_list);
	envp = malloc((count + 1) * sizeof(char *));
	if (!envp)
		return (NULL);
	current = env_list;
	i = 0;
	while (i < count)
	{
		envp[i] = create_env_entry(current);
		if (!envp[i])
		{
			return (NULL);
		}
		current = current->next;
		i++;
	}
	envp[count] = NULL;
	return (envp);
}
