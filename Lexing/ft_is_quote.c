/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grmullin <grmullin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:31:24 by grmullin          #+#    #+#             */
/*   Updated: 2024/10/09 13:31:59 by grmullin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    ft_is_single_quote(t_token **head, t_token **tmp, char *line, int *i)
{
    char    *value;
    
    if (line[*i] == '\'')
    {
        value = ft_strdup("'");
        *i += 1;
        while (line[*i] && line[*i] != '\'')
        {
            value = ft_strjoin(value, ft_substr(line, *i, 1));
            *i += 1;
        }
        value = ft_strjoin(value, "'");
        create_token(head, tmp, value, SINGLE_QUOTE);
    }
}

void    ft_is_double_quote(t_token **head, t_token **tmp, char *line, int *i)
{
    char    *value;
    
    if (line[*i] == '\"')
    {
        value = ft_strdup("\"");
        *i += 1;
        while (line[*i] && line[*i] != '\"')
        {
            value = ft_strjoin(value, ft_substr(line, *i, 1));
            *i += 1;
        }
        value = ft_strjoin(value, "\"");
        create_token(head, tmp, value, DOUBLE_QUOTE);
        *i += 1;
    }
}