/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbyrne <jbyrne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:59:11 by jbyrne            #+#    #+#             */
/*   Updated: 2024/12/16 19:59:33 by jbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_token	*parse_output_redirection(t_token *current, t_cmd *cmd)
{
	cmd->append = (current->type == REDIR_OUT_APPEND);
	current = current->next;
	if (!current || current->type != WORD)
	{
		fprintf(stderr, "Syntax error: No file specified for output redirection\n");
		return (NULL);
	}
	cmd->output_file = current->value;
	return (current->next);
}

static t_token	*parse_input_redirection(t_token *current, t_cmd *cmd)
{
	current = current->next;
	if (!current || current->type != WORD)
	{
		fprintf(stderr, "Syntax error: No file specified for input redirection\n");
		return (NULL);
	}
	cmd->input_file = current->value;
	return (current->next);
}

static t_token	*parse_heredoc_redirection(t_token *current, t_cmd *cmd)
{
	current = current->next;
	if (!current || current->type != WORD)
	{
		fprintf(stderr, "Syntax error: No delimiter specified for heredoc redirection\n");
		return (NULL);
	}
	cmd->heredoc_delimiter = current->value;
	return (current->next);
}

void	parse_redirections(t_token *token_list, t_cmd *cmd)
{
	t_token	*current = token_list;

	if (!current)
		return ((void)fprintf(stderr, "Error: no token list\n"));

	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_OUT_APPEND)
			current = parse_output_redirection(current, cmd);
		else if (current->type == REDIR_IN)
			current = parse_input_redirection(current, cmd);
		else if (current->type == HEREDOC)
			current = parse_heredoc_redirection(current, cmd);
		else if (current->type == PIPE && current->next && current->next->type == PIPE)
			return ((void)fprintf(stderr, "Syntax error: Unexpected token '||'\n"));
		else
			current = current->next;
	}
}
