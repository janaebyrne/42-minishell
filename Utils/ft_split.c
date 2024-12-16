/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grmullin <grmullin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 11:59:45 by grmullin          #+#    #+#             */
/*   Updated: 2024/10/09 16:30:13 by grmullin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**free_array(char **ptr, int i)
{
	while (i > 0)
	{
		i--;
		free(ptr[i]);
	}
	free(ptr);
	return (0);
}

static int	ft_count_words(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			i++;
		else
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

static char	*ft_putword(char *word, const char *s, int i, int word_len)
{
	int	j;

	j = 0;
	while (word_len > 0)
	{
		word[j] = s[i - word_len];
		j++;
		word_len--;
	}
	word[j] = '\0';
	return (word);
}

static char	**ft_split_enter(const char *s, char c, char **strings, int count)
{
	int	i;
	int	word;
	int	word_len;

	i = 0;
	word = 0;
	word_len = 0;
	while (word < count)
	{
		while (s[i] && s[i] == c)
			i++;
		while (s[i] && s[i] != c)
		{
			i++;
			word_len++;
		}
		strings[word] = (char *)malloc(sizeof(char) * (word_len + 1));
		if (strings == NULL)
			return (free_array(strings, word));
		ft_putword(strings[word], s, i, word_len);
		word_len = 0;
		word++;
	}
	strings[word] = 0;
	return (strings);
}

char	**ft_split(const char *s, char c)
{
	char			**strings;
	unsigned int	count;

	if (s == NULL)
		return (NULL);
	count = ft_count_words(s, c);
	strings = (char **)malloc(sizeof(char *) * (count + 1));
	if (strings == NULL)
		return (NULL);
	strings = ft_split_enter(s, c, strings, count);
	return (strings);
}
