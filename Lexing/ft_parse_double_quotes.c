/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_double_quotes.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbyrne <jbyrne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:18:59 by jbyrne            #+#    #+#             */
/*   Updated: 2024/12/16 18:25:56 by jbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*extract_var_name(const char *s, int *i)
{
	char	*var_name;
	int		k;

	var_name = malloc(256);
	k = 0;
	while (s[*i] && (isalnum(s[*i]) || s[*i] == '_'))
	{
		var_name[k++] = s[*i];
		(*i)++;
	}
	var_name[k] = '\0';
	return (var_name);
}

static void	handle_var_expansion(const char *s, char *new_value, int *i, int *j,
		t_env *env_list)
{
	char	*var_name;
	char	*value;

	(*i)++;
	var_name = extract_var_name(s, i);
	value = get_env_var(env_list, var_name);
	free(var_name);
	if (value)
	{
		while (*value)
			new_value[(*j)++] = *value++;
	}
}

void	parse_double_quotes(const char *s, char *new_value, int *i, int *j,
		t_env *env_list)
{
	(*i)++;
	while (s[*i] && s[*i] != '"')
	{
		if (s[*i] == '\\' && (s[*i + 1] == '"' || s[*i + 1] == '\\'))
			(*i)++;
		else if (s[*i] == '$')
			handle_var_expansion(s, new_value, i, j, env_list);
		else
			new_value[(*j)++] = s[(*i)++];
	}
	if (s[*i] == '"')
		(*i)++;
}
