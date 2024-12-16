/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shkaruna <shkaruna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 16:51:55 by jbyrne            #+#    #+#             */
/*   Updated: 2024/12/11 10:39:38 by shkaruna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include "../get_next_line.h"

char	*join_and_free(char *leftover, char *buffer)
{
	char	*temp;

	temp = ft_strjoin(leftover, buffer);
	free(leftover);
	return (temp);
}


void	ft_bzero(void *s, size_t n)
{
	char	*c;

	c = (char *)s;
	while (n > 0)
	{
		*c = 0;
		c++;
		n--;
	}
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*p;

	if (nmemb > 0)
	{
		if (SIZE_MAX / nmemb < size)
			return (0);
	}
	p = (void *)malloc(nmemb * size);
	if (!p)
		return (0);
	ft_bzero(p, nmemb * size);
	return (p);
}


char	*read_from_file(int fd, char *leftover)
{
	char	*buffer;
	int		bytes_read;

	if (!leftover)
		leftover = ft_calloc(1, 1);
	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(leftover);
			free(buffer);
			return (NULL);
		}
		buffer[bytes_read] = 0;
		leftover = join_and_free(leftover, buffer);
		if (ft_strchr(leftover, '\n'))
			break ;
	}
	free(buffer);
	return (leftover);
}

char	*extract_line(char *leftover)
{
	int		i;
	char	*str;

	i = 0;
	if (!leftover[i])
		return (NULL);
	while (leftover[i] && leftover[i] != '\n')
		i++;
	str = ft_calloc(i + 2, sizeof(char));
	i = 0;
	while (leftover[i] && leftover[i] != '\n')
	{
		str[i] = leftover[i];
		i++;
	}
	if (leftover[i] && leftover[i] == '\n')
		str[i++] = '\n';
	return (str);
}

char	*clean_file(char *leftover)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	while (leftover[i] && leftover[i] != '\n')
		i++;
	if (!leftover[i])
	{
		free(leftover);
		return (NULL);
	}
	str = ft_calloc((ft_strlen(leftover) - i + 1), sizeof(*leftover));
	if (!str)
		return (NULL);
	while (leftover[++i])
		str[j++] = leftover[i];
	str[j] = '\0';
	free(leftover);
	return (str);
}

char	*get_next_line(int fd)
{
	char		*output;
	static char	*leftover;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	leftover = read_from_file(fd, leftover);
	if (!leftover)
		return (NULL);
	output = extract_line(leftover);
	leftover = clean_file(leftover);
	return (output);
}



// int main(void)
// {
//     int fd;
//     char *line;

//     fd = open("test.txt", O_RDONLY);
//     if (fd < 0) {
//         perror("Error opening file");
//         return (1);
//     }
//     line = get_next_line(fd);
// 	printf("line is:%s", line);
// 	line = get_next_line(fd);
// 	printf("nextlineis:%s", line);
// 	line = get_next_line(fd);
// 	printf("nextlineis%s", line);
//     free(line);
//     close(fd);
//     return (0);
// }
