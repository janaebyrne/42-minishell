/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janaebyrne <janaebyrne@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 17:10:15 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/18 08:18:54 by janaebyrne       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int main(int argc, char **argv, char **envp)
{
    t_cmd *cmd_list = NULL;
    t_shell shell;
    shell.env_list = env_init(envp);
    shell.last_exit_status = 0;
    t_token *token_list = NULL;
    char *line;
    char **args;
    (void)argc;
    (void)argv;
    char *prompt;

    prompt = "\033[1mMinishell🐚 \033[0m";
    set_signals_interactive();
    while (1)
    {
        line = readline(prompt);
        if (!line)
        {
            write(STDOUT_FILENO, "exit\n", 5);
            break;
        }

        if (line[0] != '\0')
            add_history(line);
        
        ft_lexing(line, &token_list, shell.env_list, &shell);
        if (!token_list)
            continue;
        if (!parse_quotes_in_tokens(token_list, shell.env_list))
        {
            printf("Error: Unclosed quote in the input\n");
            free_token_list(token_list);
            token_list = NULL;
            continue;
        }
        args = ft_split(line, ' ');
        if (!args)
        {
            free_token_list(token_list);
            token_list = NULL;
            continue;
        }
        cmd_list = parse_pipeline(token_list);
        if (!cmd_list)
        {
            free_token_list(token_list);
            token_list = NULL;
            continue;
        }
        set_signals_noninteractive();
        ft_execute_main(cmd_list, &shell);
        set_signals_interactive();
        free(args);
        cmd_list = NULL;
        token_list = NULL;
    }
    return 0;
}
