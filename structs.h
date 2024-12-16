/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shkaruna <shkaruna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:47:56 by grmullin          #+#    #+#             */
/*   Updated: 2024/12/12 16:06:15 by shkaruna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "minishell.h"

typedef enum e_error_type
{
    CMD_NOT_FOUND,
    PERMISSION_DENIED,
    SYNTAX_ERROR,
    FILE_NOT_FOUND,
	INVALID_IDENTIFIER,
    // we can add more error types as needed
} 	error_type;

typedef enum e_token_type
{
	PIPE,
	REDIR_OUT,
	REDIR_OUT_APPEND,
	REDIR_IN,
	HEREDOC,
	VARIABLE,
	ASSIGNMENT, // assignments like VAR=value
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	WORD,
	WHITESPACE,
	END,
}					t_token_type;

typedef struct s_token
{
	char			*value;
	int				node;
	t_token_type	type;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_shell
{
	t_env *env_list;
	int last_exit_status;
} t_shell;

typedef struct s_cmd {
    char *command;
    char **args;
    char *input_file;
    char *output_file;
    char *heredoc_delimiter;
	struct s_env	*env_list;
    int append; // 1 for append redirection
    struct s_cmd *next;
} t_cmd;

#endif