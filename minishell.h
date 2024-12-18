/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janaebyrne <janaebyrne@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 19:41:21 by grmullin          #+#    #+#             */
/*   Updated: 2024/12/18 07:19:36 by janaebyrne       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include "structs.h"
#include <limits.h>
#include "get_next_line.h"
//# include <X11/X.h> // potentially use for keymask
//# include <X11/keysym.h> // potentially use for keymask

/*          structs         */

typedef struct s_token t_token;
typedef enum e_token_type t_token_type;
typedef struct s_node   t_node;
typedef enum e_error_type error_type;

/*          lexing          */
int         create_token(t_token **head, t_token **tmp, char *value, t_token_type type);
void        ft_is_pipe(t_token **head, t_token **tmp, char *line, int *i);
void        ft_is_redir_out(t_token **head, t_token **tmp, char *line, int *i);
void        ft_is_redir_in(t_token **head, t_token **tmp, char *line, int *i);
void        ft_is_single_quote(t_token **head, t_token **tmp, char *line, int *i);
void        ft_is_double_quote(t_token **head, t_token **tmp, char *line, int *i);
void        ft_is_whitespace(t_token **head, t_token **tmp, char *line, int *i);
void        ft_is_word(t_token **head, t_token **tmp, char *line, int *i);
int         is_char(char *line, int *i);
const char  *get_token_type(t_token_type type);
t_token     *find_next_op(t_token *token_list);
t_token     *find_prev_op(t_token *token_list);
void        print_tokens(t_token *head);

int parse_quotes_in_tokens(t_token *token_list, t_env *env_list);
void	process_token(t_token **head, t_token **tmp, char *line, int *i);

/*          parsing         */
t_cmd *parse_pipeline(t_token *token_list);
void    ft_lexing(char *line, t_token **head, t_env *env_list, t_shell *shell);
char *expand_exit_status(char *str, int last_exit_status);
t_cmd *parser(t_token *l_token);
t_cmd *parser_scmd_tokens(t_token *l_token);
int parser_cmd_group_merge(t_cmd **l_cmd);
int parser_cmd_pipeline_merge(t_cmd **l_cmd);
int parser_heredoc(t_cmd *l_cmd);
void parse_redirections(t_token *token_list, t_cmd *cmd);
void free_command_list(t_cmd *cmd_list);
void free_token_list(t_token *token_list);
void	parse_double_quotes(const char *s, char *new_value, int *i, int *j, t_env *env_list);
int	count_words_without_redirections(char *str, char delimiter);
char	**ft_split_without_redirections(char *str, char delimiter);
int	count_words_without_redirections(char *str, char delimiter);
char	**ft_split_without_redirections(char *str, char delimiter);
char	*get_next_word(char **str);



t_cmd *create_cmd_node(void);
t_cmd *parse_tokens(t_token *token_list);
t_cmd *create_cmd_node(void);

/*          Signals         */
void signal_reset_prompt(int signo);
void ignore_sigquit(void);
void set_signals_interactive(void);
void signal_print_newline(int signal);
void set_signals_noninteractive(void);

/*          Builtin Utils            */
char	*get_env_value(t_env *env_list, const char *key);
void	update_env_var(t_env **env_list, const char *key, const char *value);
// void	add_or_update_env(t_env **env_list, const char *key,
// 		const char *value);
// int	is_valid_identifier(const char *str);
// void	set_value(char **dest, const char *src);
void	alpha_sort_env_list(t_env **head);
void	print_sorted_env_list(t_env *env_list);
void	free_env_list(t_env *env_list);
t_env	*copy_env_list(t_env *env_list);
void	add_or_update_env(t_env **env_list, const char *key, const char *value);

/*			ft_split			*/
char		**ft_split(const char *s, char c);

/*			string utils			*/
int 	ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(const char *str);
char    *ft_strdup(const char *str);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char    *ft_strjoin(char const *s1, char const *s2);
char    *ft_substr(char const *s, unsigned int start, size_t len);
int	ft_strcmp(const char *s1, const char *s2);
void	ft_putstr_fd(char *s, int fd);
int	ft_is_letter(char *s);
void handle_error(error_type error_type, const char *extra_info);
char	*ft_strchr(const char *s, int c);
int      valid_quotes(const char *s);
int ft_isalnum(int c);
int ft_isalpha(int c);
char	*ft_strcpy(char *dest, const char *src);
int ft_is_digit(int c);
int	ft_atoi(const char *str);
char	*ft_itoa(int n);
size_t  ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t  ft_strlcpy(char *dst, const char *src, size_t dstsize);

/*          execution           */
// void handle_input_redirection(t_cmd *cmd, int prev_fd);
// void handle_output_redirection(t_cmd *cmd, int pipe_fd[2]);
// void handle_heredoc(char *delimiter);
// void execute_command(t_cmd *cmd, int prev_fd, int pipe_fd[2]);
// void execute_child(t_cmd *cmd, int prev_fd, int pipe_fd[2]);
// void execute_parent(int *prev_fd, int pipe_fd[2], t_cmd *cmd);


/*          Execution           */
int ft_execute_main(t_cmd *cmd_list, t_shell *shell);
int ft_execute(t_cmd *cmd_list, t_shell *shell);
//int execute_command_with_redirection(t_cmd *cmd, t_env *env_list);
//int setup_pipes_and_fork(t_cmd *cmd_list, int *pids, t_env *env_list);
int ft_execute_pipeline(t_cmd *cmd, t_shell *shell);
//void ft_execute_redirections(t_cmd *cmd, t_env *env_list);
int	count_env_list(t_env *env_list);
char	*create_env_entry(t_env *env);
char	**env_list_to_array(t_env *env_list);
char	*strip_newline(char *str);

void setup_redirection(t_cmd *node);
int open_file(const char *file, int flags, mode_t mode);
void redirect_fd(int oldfd, int newfd);
int handle_heredoc(char *delimiter, t_env *env_list);
char *expand_env(char *str, t_env *env_list);


/*          Envp                */
void	print_list(t_env *env_list);
t_env	*env_init(char **envp);
char	*get_env_var(t_env *env_list, char *key);
void	add_to_list(t_env **env_list, char *key, char *value);
int ft_execute_external_command(t_cmd *cmd_list, t_env *env_list);


/*          builtins            */
int ft_built_ins(t_cmd *cmd_list, t_env *env_list);
int     is_built_in(char *value);
int     ft_echo(t_cmd *cmd_list);
int ft_cd(t_env **env_list, const char *path);
int     ft_pwd(t_cmd *cmd_list);
int ft_export(t_cmd *cmd_list, t_env **env_list);
int ft_unset(t_cmd *cmd_list, t_env **env_list);
int ft_env(t_cmd *cmd_list, t_env *env_list);
int ft_exit(t_cmd *cmd);


char    *get_pwd(char **envp);
char    **alpha_sort(char **envp);
void    ft_swap(char **a, char **b);

/*          invalid input           */
void    command_not_found(t_node *node);

/*          main                    */
void	process_line(char *line, t_shell *shell);
void	init_shell(t_shell *shell, char **envp);
t_cmd	*prepare_commands(char *line, t_token **token_list, t_shell *shell);
// /*          get_next_line           */
// char	*get_next_line(int fd);
// char	*clean_file(char *leftover);
// char	*extract_line(char *leftover);
// char	*read_from_file(int fd, char *leftover);


#endif
