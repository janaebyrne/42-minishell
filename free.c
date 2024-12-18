/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: janaebyrne <janaebyrne@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 07:13:43 by janaebyrne        #+#    #+#             */
/*   Updated: 2024/12/18 07:14:12 by janaebyrne       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_command_list(t_cmd *cmd_list) {
    t_cmd *temp;
    while (cmd_list) {
        temp = cmd_list;
        cmd_list = cmd_list->next;
        free(temp->command);
        for (int i = 0; temp->args && temp->args[i]; i++)
            free(temp->args[i]);
        free(temp->args);
        free(temp->input_file);
        free(temp->output_file);
        free(temp);
    }
}

void free_token_list(t_token *token_list) {
    t_token *temp;
    while (token_list) {
        temp = token_list;
        token_list = token_list->next;
        free(temp->value);
        free(temp);
    }
}