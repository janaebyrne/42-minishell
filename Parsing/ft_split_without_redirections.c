/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_without_redirections.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janaebyrne <janaebyrne@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:06:12 by jbyrne            #+#    #+#             */
/*   Updated: 2024/12/18 11:05:05 by janaebyrne       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


static void	free_split(char **split)
{
	int	i;
	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}
char	*skip_whitespace(char *str, char delimiter)
{
	while (*str == ' ' || *str == delimiter)
		str++;
	return (str);
}

static char *skip_redirection(char *str)
{
	if (*(str + 1) == *(str + 2))
		str += 2;
	else
		str += 1; 
	str = skip_whitespace(str, ' ');
	return str;
}

char	**ft_split_without_redirections(char *str, char delimiter)
{
	char	**result;
	int		i;
	
	i= 0;
	result = malloc(sizeof(char *) * (count_words_without_redirections(str, delimiter) + 1));
	if (!result)
		return (NULL);
	while (*str)
	{
		if (*str == ' ' || *str == delimiter)
			str = skip_whitespace(str, delimiter);
		else if (*str == '<' || *str == '>')
			str = skip_redirection(str);
		else if (*str && !(result[i++] = get_next_word(&str)))
		{
			free_split(result);
			return (NULL);
		}
	}
	result[i] = NULL;
	return (result);
}
