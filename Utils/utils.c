/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shkaruna <shkaruna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 17:10:42 by grmullin          #+#    #+#             */
/*   Updated: 2024/11/21 15:35:48 by shkaruna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	x;

	if (n == 0)
		return (0);
	x = 0;
	while (s1[x] == s2[x] && s1[x] && x < n - 1)
		x++;
	return ((unsigned char)s1[x] - (unsigned char)s2[x]);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}
#include <stddef.h>

static void *ft_memcpy(void *dst, const void *src, size_t n)
{
    size_t i;
    unsigned char *d;
    const unsigned char *s;

    if (!dst && !src)
        return (NULL);
    d = (unsigned char *)dst;
    s = (const unsigned char *)src;
    i = 0;
    while (i < n)
    {
        d[i] = s[i];
        i++;
    }
    return (dst);
}

char *ft_strdup(const char *s1)
{
    char *copy;
    size_t len;

    if (!s1)
        return (NULL);
    len = ft_strlen(s1);
    copy = (char *)malloc(len + 1);
    if (!copy)
        return (NULL);
    ft_memcpy(copy, s1, len);
    copy[len] = '\0';
    return (copy);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*new;
	size_t	i;

	if (!s)
		return (NULL);
	new = malloc(1 + len * sizeof(char));
	if (new == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (i < len && s[start] != '\0')
	{
		new[i] = s[start];
		i++;
		start++;
	}
	new[i] = '\0';
	return (new);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	char			*cat;
	unsigned int	i;
	unsigned int	j;
	unsigned int	len;

	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	cat = (char *)malloc(sizeof(char) * len);
	if (cat == NULL || s1 == NULL || s2 == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		cat[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		cat[i++] = s2[j++];
	cat[i] = '\0';
	return (cat);
}

