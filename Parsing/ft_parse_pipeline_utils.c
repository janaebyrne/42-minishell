/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_pipeline_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbyrne <jbyrne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:35:27 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/16 20:05:06 by jbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_exit_status(char *str, int last_exit_status)
{
	char	*result;
	char	*exit_status;
	char	*dollar_question;

	dollar_question = strstr(str, "$?");
	if (!dollar_question)
		return (strdup(str));
	exit_status = ft_itoa(last_exit_status);
	result = malloc(strlen(str) + strlen(exit_status) - 2 + 1);
	if (!result)
	{
		free(exit_status);
		return (NULL);
	}
	strncpy(result, str, dollar_question - str);
	result[dollar_question - str] = '\0';
	strcat(result, exit_status);
	strcat(result, dollar_question + 2);
	free(exit_status);
	return (result);
}

char	*get_next_word(char **str)
{
	char	*start;
	char	*word;
	int		len;

	len = 0;
	while (**str && **str == ' ')
		(*str)++;
	start = *str;
	while (**str && **str != ' ' && **str != '<' && **str != '>'
		&& **str != '|')
	{
		(*str)++;
		len++;
	}
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	strncpy(word, start, len);
	word[len] = '\0';
	return (word);
}

int	count_words_without_redirections(char *str, char delimiter)
{
	int	count = 0;

	while (*str)
	{
		while (*str == ' ' || *str == delimiter)
			str++;
		if ((*str == '<' || *str == '>') && (++str, *str == '<' || *str == '>'))
			str++;
		while (*str == ' ')
			str++;
		if (*str && *str != '<' && *str != '>')
		{
			count++;
			while (*str && *str != ' ' && *str != delimiter)
				str++;
		}
	}
	return (count);
}



