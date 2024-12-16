/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbyrne <jbyrne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:50:33 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/16 18:18:39 by jbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	valid_quotes(const char *s)
{
	int	in_single_quotes;
	int	in_double_quotes;

	in_single_quotes = 0;
	in_double_quotes = 0;
	while (*s)
	{
		if (*s == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (*s == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else if (*s == '\\' && (*(s + 1) == '\'' || *(s + 1) == '"'))
			s++;
		s++;
	}
	if (in_single_quotes || in_double_quotes)
		return (0);
	return (1);
}

static void	parse_single_quotes(const char *s, char *new_value, int *i, int *j)
{
	(*i)++;
	while (s[*i] && s[*i] != '\'')
	{
		new_value[(*j)++] = s[(*i)++];
	}
	if (s[*i] == '\'')
	{
		(*i)++;
	}
}


int	parse_quotes_in_tokens(t_token *token_list, t_env *env_list)
{
	t_token	*current;
	char	*s;
	int		i;
	int		j;
	char	*new_value;

	current = token_list;
	while (current)
	{
		if (!current->value)
		{
			fprintf(stderr, "Token value is NULL at parse_quotes_in_tokens\n");
			current = current->next;
			continue ;
		}
		if (current->type == WORD)
		{
			s = current->value;
			if (!valid_quotes(s))
			{
				return (0);
			}
			new_value = malloc(strlen(s) + 1);
			if (!new_value)
			{
				fprintf(stderr, "Memory allocation error\n");
				exit(EXIT_FAILURE);
			}
			i = 0;
			j = 0;
			while (s[i])
			{
				if (s[i] == '\'')
					parse_single_quotes(s, new_value, &i, &j);
				else if (s[i] == '"')
					parse_double_quotes(s, new_value, &i, &j, env_list);
				else
					new_value[j++] = s[i++];
			}
			new_value[j] = '\0';
			free(current->value);
			current->value = new_value;
		}
		current = current->next;
	}
	return (1);
}
