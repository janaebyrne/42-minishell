/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shkaruna <shkaruna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 15:52:39 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/15 22:35:20 by shkaruna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	change_directory(const char *path)
{
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}

static int	handle_home_directory(t_env **env_list, const char **path)
{
	char	*home_dir;

	home_dir = get_env_value(*env_list, "HOME");
	if (!home_dir)
	{
		fprintf(stderr, "minishell: cd: HOME not set\n");
		return (1);
	}
	*path = home_dir;
	return (0);
}

static int	handle_oldpwd(t_env **env_list, const char **path)
{
	char	*oldpwd;

	oldpwd = get_env_value(*env_list, "OLDPWD");
	if (!oldpwd)
	{
		fprintf(stderr, "minishell: cd: OLDPWD not set\n");
		return (1);
	}
	printf("%s\n", oldpwd);
	*path = oldpwd;
	return (0);
}

static void	update_environment_variables(t_env **env_list)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		update_env_var(env_list, "OLDPWD", get_env_value(*env_list, "PWD"));
		update_env_var(env_list, "PWD", cwd);
		free(cwd);
	}
	else
		perror("minishell: cd: getcwd");
}

int	ft_cd(t_env **env_list, const char *path)
{
	if (!path || strcmp(path, "~") == 0)
	{
		if (handle_home_directory(env_list, &path) != 0)
			return (1);
	}
	else if (ft_strcmp(path, "-") == 0)
	{
		if (handle_oldpwd(env_list, &path) != 0)
			return (1);
	}
	if (change_directory(path) != 0)
		return (1);
	update_environment_variables(env_list);
	return (0);
}
