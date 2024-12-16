
#include "../../minishell.h"

static int	count_env_list(t_env *env_list)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static char	*create_env_entry(t_env *env)
{
	int		len;
	char	*entry;

	len = ft_strlen(env->key) + ft_strlen(env->value) + 2;
	entry = malloc(len);
	if (!entry)
		return (NULL);
	sprintf(entry, "%s=%s", env->key, env->value);
	return (entry);
}

static char	**env_list_to_array(t_env *env_list)
{
	int		count;
	t_env	*current;
	char	**envp;
	int		i;

	count = count_env_list(env_list);
	envp = malloc((count + 1) * sizeof(char *));
	if (!envp)
		return (NULL);
	current = env_list;
	i = 0;
	while (i < count)
	{
		envp[i] = create_env_entry(current);
		if (!envp[i])
		{
			// free_envp(envp, i);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	envp[count] = NULL;
	return (envp);
}

static char	*try_command_in_path(char *path, const char *command)
{
	char	*temp;
	size_t	len;

	len = ft_strlen(path) + ft_strlen(command) + 2;
	temp = malloc(len);
	if (!temp)
		return (NULL);
	ft_strlcpy(temp, path, len);
	ft_strlcat(temp, "/", len);
	ft_strlcat(temp, command, len);
	return (temp);
}

static char	*find_command_path(char *path_copy, const char *command)
{
	char	*token;
	char	*next_token;
	char	*full_path;

	token = path_copy;
	full_path = NULL;
	while (token)
	{
		next_token = ft_strchr(token, ':');
		if (next_token)
			*next_token = '\0';
		full_path = try_command_in_path(token, command);
		if (!full_path)
			break ;
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		if (next_token)
			token = next_token + 1;
		else
			break ;
	}
	return (NULL);
}

static char	*resolve_command_path(const char *command, t_env *env_list)
{
	char	*path_env;
	char	*path_copy;
	char	*full_path;

	(void)env_list;
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	path_copy = ft_strdup(path_env);
	if (!path_copy)
		return (NULL);
	full_path = find_command_path(path_copy, command);
	free(path_copy);
	return (full_path);
}

static void	execute_child_process(t_cmd *cmd_list, t_env *env_list)
{
	char	*path;
	char	**args;
	char	**envp;

	setup_redirection(cmd_list);
	path = resolve_command_path(cmd_list->command, env_list);
	if (!path)
	{
		fprintf(stderr, "Command not found: %s\n", cmd_list->command);
		_exit(127);
	}
	args = cmd_list->args;
	envp = env_list_to_array(env_list);
	if (!envp)
	{
		perror("Failed to create environment array");
		exit(EXIT_FAILURE);
	}
	execve(path, args, envp);
	perror("execve failed");
	exit(EXIT_FAILURE);
}

int	ft_execute_external_command(t_cmd *cmd_list, t_env *env_list)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("Fork failed");
		return (-1);
	}
	else if (pid == 0)
	{
		execute_child_process(cmd_list, env_list);
	}
	else
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	return (0);
}
