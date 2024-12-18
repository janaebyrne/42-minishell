/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janaebyrne <janaebyrne@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:58:01 by jbyrne            #+#    #+#             */
/*   Updated: 2024/12/18 07:06:45 by janaebyrne       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	init_new_cmd(t_cmd *new_cmd)
{
	new_cmd->command = NULL;
	new_cmd->args = NULL;
	new_cmd->input_file = NULL;
	new_cmd->output_file = NULL;
	new_cmd->heredoc_delimiter = NULL;
	new_cmd->append = 0;
	new_cmd->next = NULL;
}

static t_cmd	*create_new_cmd(t_cmd *head, t_token *tokens)
{
	t_cmd	*new_cmd;
	char	**args;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
	{
		perror("malloc");
		free_command_list(head);
		return (NULL);
	}
	init_new_cmd(new_cmd);
	parse_redirections(tokens, new_cmd);
	args = ft_split_without_redirections(tokens->value, ' ');
	if (!args)
	{
		free(new_cmd);
		free_command_list(head);
		return (NULL);
	}
	new_cmd->command = args[0];
	new_cmd->args = args;
	return (new_cmd);
}

t_cmd	*parse_pipeline(t_token *token_list)
{
	t_cmd	*head;
	t_cmd	*current;
	t_token	*tokens;

	head = NULL;
	current = NULL;
	tokens = token_list;
	while (tokens)
	{
		t_cmd *new_cmd = create_new_cmd(head, tokens);
		if (!new_cmd)
			return (NULL);
		if (!head)
			head = new_cmd;
		else
			current->next = new_cmd;
		current = new_cmd;
		while (tokens && tokens->type != PIPE)
			tokens = tokens->next;
		if (tokens)
			tokens = tokens->next;
	}
	return (head);
}
