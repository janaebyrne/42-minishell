/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbyrne <jbyrne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:40:01 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/16 16:39:58 by jbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

#include "../../minishell.h"

static int is_valid_identifier(const char *str)
{
    if (!ft_isalpha(str[0]) && str[0] != '_')
        return (0);
    for (int i = 1; str[i] != '\0' && str[i] != '='; i++)
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
    }
    return (1);
}

static void add_or_update_env(t_env **env_list, const char *key, const char *value)
{
    t_env *current;

    current = *env_list;
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            free(current->value);
            current->value = value ? ft_strdup(value) : NULL;
            return;
        }
		if (!current->next)
			break;
        current = current->next;
    }
    t_env *new_node = malloc(sizeof(t_env));
    if (!new_node)
        return;
    new_node->key = ft_strdup(key);
    new_node->value = value ? ft_strdup(value) : NULL;
    new_node->next =  NULL;

	if (!*env_list)
		*env_list = new_node;
	else
		current->next = new_node;
}

static void handle_argument(t_env **env_list, const char *arg)
{
    char *key = ft_strdup(arg);
    char *value = ft_strchr(key, '=');

    if (value)
    {
        *value = '\0';
        value++;
        if (!is_valid_identifier(key))
        {
            printf("export: `%s': not a valid identifier\n", arg);
            free(key);
            return;
        }
        add_or_update_env(env_list, key, value);
    }
    else
    {
        if (!is_valid_identifier(key))
        {
            printf("export: `%s': not a valid identifier\n", key);
        }
        else
        {
            add_or_update_env(env_list, key, NULL);
        }
    }
    free(key);
}

static t_env *copy_env_list(t_env *env_list)
{
    t_env *new_list = NULL;
    t_env *current = env_list;
    t_env *new_node;

    while (current)
    {
        new_node = malloc(sizeof(t_env));
        if (!new_node)
            return NULL;
        new_node->key = ft_strdup(current->key);
        new_node->value = current->value ? ft_strdup(current->value) : NULL;
        new_node->next = new_list;
        new_list = new_node;
        current = current->next;
    }
    return new_list;
}

static void free_env_list(t_env *env_list)
{
    t_env *temp;

    while (env_list)
    {
        temp = env_list;
        env_list = env_list->next;
        free(temp->key);
        free(temp->value);
        free(temp);
    }
}

static void alpha_sort_env_list(t_env **head)
{
    if (!head || !*head)
        return;

    int swapped;
    t_env *i;
    t_env *j = NULL;
    do
    {
        swapped = 0;
        i = *head;
        while (i->next != j)
        {
            if (ft_strcmp(i->key, i->next->key) > 0)
            {
                char *temp_key = i->key;
                char *temp_value = i->value;
                i->key = i->next->key;
                i->value = i->next->value;
                i->next->key = temp_key;
                i->next->value = temp_value;
                swapped = 1;
            }
            i = i->next;
        }
        j = i;
    } while (swapped);
}

static void print_sorted_env_list(t_env *env_list)
{
    t_env *sorted_list = copy_env_list(env_list);
    if (!sorted_list)
        return;

    alpha_sort_env_list(&sorted_list);

    t_env *current = sorted_list;
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

int ft_export(t_cmd *cmd_list, t_env **env_list)
{
    if (!cmd_list->args[1])
    {
        print_sorted_env_list(*env_list);
        return (0);
    }
    int i = 1;
    while (cmd_list->args[i])
    {
        handle_argument(env_list, cmd_list->args[i]);
        i++;
    }
    return (0);
}
