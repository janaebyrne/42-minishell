/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbyrne <jbyrne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:13:22 by jbyrne            #+#    #+#             */
/*   Updated: 2024/12/16 20:53:57 by jbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*ft_strndup(const char *str, size_t n)
{
	size_t	i;
	char	*dup;

	dup = (char *)malloc((n + 1) * sizeof(char));
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (i < n && str[i] != '\0')
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

static size_t	calculate_result_size(char *str, t_env *env_list)
{
	size_t	result_size;
	char	*start;
	char	*end;
	char	*var_name;
	char	*var_value;

	result_size = 1;
	start = str;
	end = ft_strchr(start, '$');
	while (end != NULL)
	{
		result_size += end - start;
		start = end + 1;
		while (*start && (isalnum(*start) || *start == '_'))
			start++;
		var_name = ft_strndup(end + 1, start - end - 1);
		var_value = get_env_value(env_list, var_name);
		if (var_value)
			result_size += ft_strlen(var_value);
		free(var_name);
	}
	result_size += ft_strlen(start);
	return (result_size);
	end = ft_strchr(start, '$');
}

static void	append_env_value(char **result, char *start, char *end,
		t_env *env_list)
{
	size_t	len;
	char	*var_name;
	char	*var_value;

	strncat(*result, start, end - start);
	start = end + 1;
	while (*start && (isalnum(*start) || *start == '_'))
		start++;
	len = start - end - 1;
	var_name = strndup(end + 1, len);
	var_value = get_env_value(env_list, var_name);
	if (var_value)
		strcat(*result, var_value);
	free(var_name);
}

char	*expand_env(char *str, t_env *env_list)
{
	size_t	result_size;
	char	*result;
	char	*start;
	char	*end;

	result_size = calculate_result_size(str, env_list);
	result = malloc(result_size);
	if (!result)
		return (NULL);
	result[0] = '\0';
	start = str;
	end = ft_strchr(start, '$');
	while (end != NULL)
	{
		append_env_value(&result, start, end, env_list);
		start = end + 1;
		end = ft_strchr(start, '$');
	}
	strcat(result, start);
	return (result);
}
