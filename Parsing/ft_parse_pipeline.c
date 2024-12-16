/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shkaruna <shkaruna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:35:27 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/12 17:53:42 by shkaruna         ###   ########.fr       */
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



// static int check_pipe_syntax(char *line)
// {
//     int i = 0;
//     int last_was_pipe = 1; // Track if the last significant character was a pipe

//     while (line[i])
//     {
//         // Skip spaces
//         while (line[i] == ' ')
//             i++;

//         // Check for invalid pipe syntax
//         if (line[i] == '|')
//         {
//             if (last_was_pipe)
//             {
//                 write(STDERR_FILENO, "minishell: syntax error near unexpected token '|'\n", 51);
//                 return 1;
//             }
//             last_was_pipe = 1; // Mark that a pipe was found
//         }
//         else if (line[i] != '\0')
//         {
//             last_was_pipe = 0; // Reset if a valid character is found
//         }

//         i++;
//     }

//     // Check if the last character is a pipe
//     if (last_was_pipe)
//     {
//         write(STDERR_FILENO, "minishell: syntax error near unexpected token '|'\n", 51);
//         return 1;
//     }

//     return 0;
// }

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
        // Move to the next token
        current = current->next;
    }
}


// static void parse_redirections(char *part, t_cmd *cmd)
// {
//     int i = 0;
//     char *file = NULL;

//     while (part[i])
//     {
//         printf("PART: %c", part[i]);//debug
//         // Check for input redirection (`<`)
//         if (part[i] == '<')
//         {
//             int is_heredoc = (part[i + 1] == '<');
//             i += is_heredoc ? 2 : 1;
//             while (part[i] == ' ') i++; // Skip spaces
//             file = get_next_word(&part); // Extract filename or delimiter

//             if (!file)
//                 return; // Handle syntax errors for missing files

//             if (is_heredoc)
//                 cmd->heredoc_delimiter = file; // Store heredoc delimiter
//             else
//                 cmd->input_file = file; // Store input file
//         }
//         // Check for output redirection (`>` or `>>`)
//         else if (part[i] == '>')
//         {
//             int is_append = (part[i + 1] == '>');
//             i += is_append ? 2 : 1;
//             printf("you are here in parse redirection 2\n");//debug
//             while (part[i] == ' ') i++; // Skip spaces
//             file = get_next_word(&part); // Extract filename
//             printf("file: %s\n", file);//debug
//             if (!file)
//                 return; // Handle syntax errors for missing files

//             cmd->output_file = file; // Store output file
//             cmd->append = is_append; // Set append flag
//         }
//         else
//             i++; // Continue parsing
//     }
// }

t_cmd *parse_pipeline(t_token *token_list)
{
    t_cmd *head = NULL;
    t_cmd *current = NULL;
    t_token *tokens = token_list;  // We work with the tokens directly
    
    while (tokens)
    {
        t_cmd *new_cmd = malloc(sizeof(t_cmd));
        if (!new_cmd)
        {
            perror("malloc");
            free_command_list(head); // Clean up previously allocated commands
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

        if (tokens) // Skip the pipe symbol
            tokens = tokens->next;
    }

    return head;
}

