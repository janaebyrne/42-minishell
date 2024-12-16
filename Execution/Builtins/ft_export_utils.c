/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbyrne <jbyrne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 20:07:37 by jbyrne            #+#    #+#             */
/*   Updated: 2024/12/16 20:36:31 by jbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_env_list(t_env *env_list)
{
	t_env	*temp;

	while (env_list)
	{
		temp = env_list;
		env_list = env_list->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}

static void	swap_env_nodes(t_env *a, t_env *b)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = a->key;
	temp_value = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = temp_key;
	b->value = temp_value;
}

void	alpha_sort_env_list(t_env **head)
{
	t_env	*i;
	t_env	*j;
	int		swapped;

	j = NULL;
	swapped = 1;
	if (!head || !*head)
		return ;
	while (swapped)
	{
		swapped = 0;
		i = *head;
		while (i->next != j)
		{
			if (ft_strcmp(i->key, i->next->key) > 0)
			{
				swap_env_nodes(i, i->next);
				swapped = 1;
			}
			i = i->next;
		}
		j = i;
	}
}

void	print_sorted_env_list(t_env *env_list)
{
	t_env	*sorted_list;
	t_env	*current;

	sorted_list = copy_env_list(env_list);
	if (!sorted_list)
		return ;
	alpha_sort_env_list(&sorted_list);
	current = sorted_list;
	while (current)
	{
		printf("declare -x %s", current->key);
		if (current->value)
			printf("=\"%s\"", current->value);
		printf("\n");
		current = current->next;
	}
	free_env_list(sorted_list);
}
