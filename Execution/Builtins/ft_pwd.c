/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shkaruna <shkaruna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:07:35 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/15 23:24:29 by shkaruna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_pwd(t_cmd *cmd_list)
{
	char	*cwd;
	char	buffer[1024];

	(void)cmd_list;
	cwd = getcwd(buffer, sizeof(buffer));
	if (cwd == NULL)
	{
		printf("Error getting the CWD\n");
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}
