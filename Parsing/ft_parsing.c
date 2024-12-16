/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shkaruna <shkaruna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 10:21:25 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/05 16:44:07 by shkaruna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../minishell.h"

// t_cmd *create_cmd_node(void)
// {
//     t_cmd *cmd = (t_cmd *)malloc(sizeof(t_cmd));
//     if (!cmd)
//         return (NULL);
//     cmd->command = NULL;
//     cmd->args = NULL;
//     cmd->input_file = NULL;
//     cmd->output_file = NULL;
//     cmd->heredoc_delimiter = NULL;
//     cmd->env_list = NULL;
//     cmd->append = 0;
//     cmd->next = NULL;
//     return (cmd);
// }

// t_cmd   *parse_command(t_token **current_token)
// {
//     t_cmd *cmd;

//     cmd = create_cmd_node();
//     while(*current_token && (*current_token)->type != PIPE)
//     {
//         if((*current_token)->type == WORD)
//         {
//             if(!cmd->command)
//             {
//                 cmd->command = ft_strdup((*current_token)->value);
//                 add_argument(cmd, (*current_token)->value);
//             }
//             else
//             {
//                 add_argument(cmd, (*current_token)->value);
//             }
//         }
//         else
//         {
//             handle_redirection(cmd, current_token);
//         }
//         *current_token = (*current_token)->next;
//     }
//     return (cmd);
// }

// t_cmd  *parse_input(t_token *token_list)
// {
//     t_cmd *cmd_list;
//     t_cmd *current_cmd;
//     t_token *current_token;

//     cmd_list = NULL;
//     current_cmd = NULL;
//     current_token = token_list;
//     while(current_token)
//     {
        
//     }
// }
// void add_argument(t_cmd *cmd, char *arg)
// {
//     int i = 0;

//     if (!cmd->args)
//     {
//         cmd->args = (char **)malloc(sizeof(char *) * 2);
//         cmd->args[0] = ft_strdup(arg);
//         cmd->args[1] = NULL;
//     }
//     else
//     {
//         while (cmd->args[i])
//             i++;
//         cmd->args = (char **)realloc(cmd->args, sizeof(char *) * (i + 2));
//         cmd->args[i] = ft_strdup(arg);
//         cmd->args[i + 1] = NULL;
//     }
// }

// void handle_redirection(t_cmd *cmd, t_token **current_token)
// {
//     if ((*current_token)->type == REDIR_IN)
//     {
//         *current_token = (*current_token)->next;
//         if (*current_token && (*current_token)->type == WORD)
//             cmd->input_file = ft_strdup((*current_token)->value);
//     }
//     else if ((*current_token)->type == REDIR_OUT)
//     {
//         *current_token = (*current_token)->next;
//         if (*current_token && (*current_token)->type == WORD)
//             cmd->output_file = ft_strdup((*current_token)->value);
//     }
//     else if ((*current_token)->type == REDIR_OUT_APPEND)
//     {
//         *current_token = (*current_token)->next;
//         if (*current_token && (*current_token)->type == WORD)
//         {
//             cmd->output_file = ft_strdup((*current_token)->value);
//             cmd->append = 1;
//         }
//     }
// }

// t_cmd *parse_tokens(t_token *token_list)
// {
//     t_cmd *cmd_list = NULL;
//     t_cmd *current_cmd = NULL;
//     t_token *current_token = token_list;

//     while (current_token)
//     {
//         if (current_token->type == PIPE)
//         {
//             if (current_cmd)
//             {
//                 current_cmd->next = create_cmd_node();
//                 current_cmd = current_cmd->next;
//             }
//         }
//         else
//         {
//             if (!current_cmd)
//             {
//                 cmd_list = create_cmd_node();
//                 current_cmd = cmd_list;
//             }
//             if (current_token->type == WORD)
//             {
//                 if (!current_cmd->command)
//                 {
//                     current_cmd->command = ft_strdup(current_token->value);
//                     add_argument(current_cmd, current_token->value);
//                 }
//                 else
//                 {
//                     add_argument(current_cmd, current_token->value);
//                 }
//             }
//             else
//             {
//                 handle_redirection(current_cmd, &current_token);
//             }
//         }
//         current_token = current_token->next;
//     }

//     return (cmd_list);
// }

// t_cmd *parse_pipeline(char *line)
// {
//     t_token *token_list = NULL;

//     ft_lexing(line, &token_list);
//     return parse_tokens(token_list);
// }

// t_cmd *parser(t_token *l_token) {
//     // Parse tokens into simple commands
//     t_cmd *l_cmd = parser_scmd_tokens(l_token);
//     if (!l_cmd) {
//         fprintf(stderr, "Error: Failed to parse simple commands.\n");
//         return NULL;
//     }

//     // Merge commands into groups if needed
//     if (parser_cmd_group_merge(&l_cmd) < 0) {
//         fprintf(stderr, "Error: Failed to merge command groups.\n");
//         return NULL;
//     }

//     // Merge commands in pipelines
//     if (parser_cmd_pipeline_merge(&l_cmd) < 0) {
//         fprintf(stderr, "Error: Failed to merge pipelines.\n");
//         return NULL;
//     }

//     // Handle heredocs
//     if (parser_heredoc(l_cmd) < 0) {
//         fprintf(stderr, "Error: Failed to handle heredocs.\n");
//         return NULL;
//     }

//     return l_cmd;
// }

// t_cmd *parser_scmd_tokens(t_token *l_token) {
//     t_cmd *cmd_list = NULL;
//     t_cmd *current_cmd = NULL;

//     while (l_token) {
//         // Skip whitespace tokens
//         if (l_token->type == WHITESPACE) {
//             l_token = l_token->next;
//             continue;
//         }

//         // Create a new command structure
//         t_cmd *cmd = malloc(sizeof(t_cmd));
//         if (!cmd) {
//             perror("malloc");
//             return NULL;
//         }

//         // Initialize the command structure
//         cmd->command = strdup(l_token->value);
//         cmd->args = NULL;  // Populate later if arguments are found
//         cmd->input_file = NULL;
//         cmd->output_file = NULL;
//         cmd->append = 0;
//         cmd->env_list = NULL;
//         cmd->next = NULL;

//         // Link to the command list
//         if (!cmd_list) {
//             cmd_list = cmd;
//         } else {
//             current_cmd->next = cmd;
//         }
//         current_cmd = cmd;

//         l_token = l_token->next;
//     }

//     return cmd_list;
// }

// int parser_cmd_group_merge(t_cmd **l_cmd) {
//     t_cmd *current = *l_cmd;

//     while (current) {
//         // Placeholder for actual group merging logic
//         if (strcmp(current->command, "(") == 0) {
//             fprintf(stderr, "Error: Grouping not implemented.\n");
//             return -1;
//         }
//         current = current->next;
//     }

//     return 0;
// }

// int parser_cmd_pipeline_merge(t_cmd **l_cmd) {
//     t_cmd *current = *l_cmd;

//     while (current && current->next) {
//         if (current->next->command && strcmp(current->next->command, "|") == 0) {
//             t_cmd *pipeline_cmd = current->next->next;
//             if (!pipeline_cmd) {
//                 fprintf(stderr, "Error: Incomplete pipeline.\n");
//                 return -1;
//             }

//             current->next = pipeline_cmd;  // Link the next command in the pipeline
//         }
//         current = current->next;
//     }

//     return 0;
// }

// int parser_heredoc(t_cmd *l_cmd) {
//     t_cmd *current = l_cmd;

//     while (current) {
//         if (current->command && strcmp(current->command, "<<") == 0) {
//             fprintf(stderr, "Error: Heredoc handling not implemented.\n");
//             return -1;
//         }
//         current = current->next;
//     }

//     return 0;
// }
