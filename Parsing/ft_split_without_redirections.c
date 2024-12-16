/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_without_redirections.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbyrne <jbyrne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 19:06:12 by jbyrne            #+#    #+#             */
/*   Updated: 2024/12/16 19:50:39 by jbyrne           ###   ########.fr       */
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
char	**ft_split_without_redirections(char *str, char delimiter)
{
	char	**result;
	int		i = 0;

	result = malloc(sizeof(char *) * (count_words_without_redirections(str, delimiter) + 1));
	if (!result) return (NULL);

	while (*str)
	{
		str = (*str == ' ' || *str == delimiter) ? skip_whitespace(str, delimiter) : str;
		if ((*str == '<' || *str == '>') && !(str += (*str + 1 == *str + 2 ? 2 : 1)))
			str = skip_whitespace(str, ' ');
		else if (*str && !(result[i++] = get_next_word(&str)))
		{
			free_split(result);
			return (NULL);
		}
	}
	result[i] = NULL;
	return (result);
}

