/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbyrne <jbyrne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:35:27 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/16 15:43:31 by jbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../minishell.h"

char *expand_exit_status(char *str, int last_exit_status)
{
    char *result;
    char *exit_status;
    char *dollar_question;

    dollar_question = strstr(str, "$?");
    if (!dollar_question)
        return strdup(str); // No `$?` in the string

    // Convert the exit status to a string
    exit_status = ft_itoa(last_exit_status); // Use your implementation of itoa

    // Allocate memory for the new string
    result = malloc(strlen(str) + strlen(exit_status) - 2 + 1); // Adjust size for `$?`
    if (!result)
    {
        free(exit_status);
        return NULL;
    }

    // Construct the new string
    strncpy(result, str, dollar_question - str); // Copy before `$?`
    result[dollar_question - str] = '\0';
    strcat(result, exit_status);                // Append exit status
    strcat(result, dollar_question + 2);        // Append after `$?`

    free(exit_status);
    return result;
}

static int count_words_without_redirections(char *str, char delimiter);
static void free_split(char **split)
{
    int i = 0;

    if (!split)
        return;

    while (split[i])
    {
        free(split[i]);
        i++;
    }
    free(split);
}

static char *get_next_word(char **str)
{
    char *start;
    char *word;
    int len = 0;

    // Skip leading spaces
    while (**str && **str == ' ')
        (*str)++;

    // Start of the word
    start = *str;

    // Calculate the length of the word (stop at space or redirection token)
    while (**str && **str != ' ' && **str != '<' && **str != '>' && **str != '|')
    {
        (*str)++;
        len++;
    }

    // Allocate memory for the word
    word = malloc(len + 1);
    if (!word)
        return NULL;

    // Copy the word into the allocated memory
    strncpy(word, start, len);
    word[len] = '\0';

    return word;
}

static char **ft_split_without_redirections(char *str, char delimiter)
{
    char **result;
    char *word;
    //int count = 0;
    int i = 0;

    // Estimate the number of words (split by delimiter, excluding redirections)
    result = malloc(sizeof(char *) * (count_words_without_redirections(str, delimiter) + 1));
    if (!result)
        return NULL;

    while (*str)
    {
        // Skip spaces and delimiters
        while (*str == ' ' || *str == delimiter)
            str++;

        // Handle redirection tokens
        if (*str == '<' || *str == '>')
        {
            if (*(str + 1) == '<' || *(str + 1) == '>')
                str += 2; // Skip `<<` or `>>`
            else
                str++; // Skip `<` or `>`
            while (*str == ' ')
                 str++; // Skip spaces
            get_next_word(&str);       // Skip the filename
            continue;
        }

        // Extract the next word
        if (*str)
        {
            word = get_next_word(&str);
            if (!word)
            {
                free_split(result); // Free previously allocated memory
                return NULL;
            }
            result[i++] = word;
        }
    }

    result[i] = NULL; // Null-terminate the array
    return result;
}

static int count_words_without_redirections(char *str, char delimiter)
{
    int count = 0;

    while (*str)
    {
        // Skip spaces and delimiters
        while (*str == ' ' || *str == delimiter)
            str++;

        // Handle redirection tokens
        if (*str == '<' || *str == '>')
        {
            if (*(str + 1) == '<' || *(str + 1) == '>')
                str += 2; // Skip `<<` or `>>`
            else
                str++; // Skip `<` or `>`
            while (*str == ' ') str++; // Skip spaces
            get_next_word(&str);       // Skip the filename
            continue;
        }

        // Count a word if it's not empty
        if (*str)
        {
            count++;
            while (*str && *str != ' ' && *str != delimiter)
                str++;
        }
    }

    return count;
}


void parse_redirections(t_token *token_list, t_cmd *cmd)
{
    t_token *current = token_list;
    while (current)
    {
        // If we find a redirection output token (>)
        if (current->type == REDIR_OUT || current->type == REDIR_OUT_APPEND)
        {
            if (current->type == REDIR_OUT_APPEND)
                cmd->append = 1;
            current = current->next;  // Move to the next token, which should be the filename
            if (current && current->type == WORD)
            {
                // The next token should be a filename for output redirection
                cmd->output_file = current->value;
            }
            else
            {
                // Handle error: Missing file for redirection
                fprintf(stderr, "Syntax error: No file specified for output redirection\n");
                return;
            }
        }
        // If we find a redirection input token (<)
        else if (current->type == REDIR_IN)
        {
            current = current->next;  // Move to the next token, which should be the filename
            if (current && current->type == WORD)
            {
                // The next token should be a filename for input redirection
                cmd->input_file = current->value;
            }
            else
            {
                // Handle error: Missing file for redirection
                fprintf(stderr, "Syntax error: No file specified for input redirection\n");
                return;
            }
        }
        else if (current->type == HEREDOC)
        {
            current = current->next;  // Move to the next token, which should be the delimiter
            if (current && current->type == WORD)
            {
                // The next token should be a delimiter for heredoc redirection
                cmd->heredoc_delimiter = current->value;
                printf("Heredoc redirection with delimiter: %s\n", cmd->heredoc_delimiter);
            }
            else
            {
                // Handle error: Missing delimiter for heredoc redirection
                fprintf(stderr, "Syntax error: No delimiter specified for heredoc redirection\n");
                return;
            }
        }
		if (current->type == PIPE && current->next && current->next->type == PIPE)
        {
            fprintf(stderr, "Syntax error: Unexpected token '||'\n");
            return;
        }
        // Move to the next token
        current = current->next;
    }
}



t_cmd *parse_pipeline(t_token *token_list)
{
    t_cmd *head = NULL;
    t_cmd *current = NULL;
    t_token *tokens = token_list;

    while (tokens)
    {
        t_cmd *new_cmd = malloc(sizeof(t_cmd));
        if (!new_cmd)
        {
            perror("malloc");
            free_command_list(head);
            return NULL;
        }

        // Initialize the new command structure
        new_cmd->command = NULL;
        new_cmd->args = NULL;
        new_cmd->input_file = NULL;
        new_cmd->output_file = NULL;
        new_cmd->heredoc_delimiter = NULL;
        new_cmd->append = 0;
        new_cmd->next = NULL;

        // Parse redirections in the current command part
        parse_redirections(tokens, new_cmd);
        // Now split the arguments (everything that's not redirection)
        char **args = ft_split_without_redirections(tokens->value, ' ');
        if (!args)
        {
            free(new_cmd);
            free_command_list(head); // Clean up previously allocated commands
            return NULL;
        }

        new_cmd->command = args[0];  // The first argument is the command
        new_cmd->args = args;

        // Link the command to the pipeline list
        if (!head)
            head = new_cmd;
        else
            current->next = new_cmd;

        current = new_cmd;

        // Move the token list forward to the next command part (before the next pipe '|')
        while (tokens && tokens->type != PIPE)
            tokens = tokens->next;

        if (tokens)
		{
			tokens = tokens->next; // Skip the single PIPE
			if (tokens && tokens->type == PIPE) // Check for double PIPE
			{
				tokens = tokens->next; // Advance past the second PIPE
				return(head); // break if double pipe found
			}
		}
}
    return (head);
}

