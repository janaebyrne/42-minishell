/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_op.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janaebyrne <janaebyrne@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:34:15 by grmullin          #+#    #+#             */
/*   Updated: 2024/12/09 01:44:40 by janaebyrne       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    ft_is_pipe(t_token **head, t_token **tmp, char *line, int *i)
{
    if (line[*i] == '|')
    {
        create_token(head, tmp, "|", PIPE);
        *i += 1;
    }
}

void    ft_is_redir_out(t_token **head, t_token **tmp, char *line, int *i)
{
    if (line[*i] == '>')
    {
        if (line[*i + 1] == '>')
        {
            create_token(head, tmp, ">>", REDIR_OUT_APPEND);
            *i += 2;
        }
        else
        {
            create_token(head, tmp, ">", REDIR_OUT);
            *i += 1;
        }
    }
}

void    ft_is_redir_in(t_token **head, t_token **tmp, char *line, int *i)
{
    if (line[*i] == '<')
    {
        if(line[*i + 1] == '<')
        {
            create_token(head, tmp, "<<", HEREDOC);
            *i += 2;
        }
        else
        {
           create_token(head, tmp, "<", REDIR_IN);
            *i += 1;
        }
    }
}



