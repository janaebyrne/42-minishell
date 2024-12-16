/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbyrne <jbyrne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 12:38:22 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/16 18:53:12 by jbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

const char	*g_token_type[] = {
	"PIPE",
	"REDIR_OUT",
	"REDIR_OUT_APPEND",
	"REDIR_IN",
	"HEREDOC",
	"VARIABLE",
	"ASSIGNMENT",
	"SINGLE_QUOTE",
	"DOUBLE_QUOTE",
	"WORD",
	"WHITESPACE",
};

const char	*get_token_type(t_token_type type)
{
	return (g_token_type[type]);
}

void	ft_lexing(char *line, t_token **head, t_env *env_list, t_shell *shell)
{
	t_token	*tmp;
	int		i;
	char	*expanded_line;

	if (!line || !head)
		return ;
	expanded_line = expand_exit_status(line, shell->last_exit_status);
	if (!expanded_line)
	{
		free(line);
		return ;
	}
	free(line);
	line = expanded_line;
	i = 0;
	tmp = NULL;
	*head = NULL;
	// printf("Lexing input: %s\n", line);
	while (line[i])
	{
		ft_is_pipe(head, &tmp, line, &i);
		ft_is_redir_out(head, &tmp, line, &i);
		ft_is_redir_in(head, &tmp, line, &i);
		ft_is_single_quote(head, &tmp, line, &i);
		ft_is_double_quote(head, &tmp, line, &i);
		ft_is_whitespace(head, &tmp, line, &i);
		ft_is_word(head, &tmp, line, &i);
	}
	parse_quotes_in_tokens(*head, env_list);
}

void	print_tokens(t_token *token_list)
{
	t_token	*tmp;

	tmp = token_list;
	while (tmp)
	{
		printf("value: %s\t", tmp->value);
		printf("type: %s\n", get_token_type(tmp->type));
		tmp = tmp->next;
	}
}

int	create_token(t_token **head, t_token **tmp, char *value, t_token_type type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (-1);
	token->value = value;
	token->type = type;
	token->node = 0;
	token->prev = NULL;
	if (*head == NULL)
	{
		*head = token;
		*tmp = *head;
	}
	else
	{
		token->prev = *tmp;
		(*tmp)->next = token;
		*tmp = (*tmp)->next;
	}
	return (0);
}
