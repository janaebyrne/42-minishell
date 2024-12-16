/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbyrne <jbyrne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:50:33 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/16 16:07:10 by jbyrne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int      valid_quotes(const char *s)
{
    int in_single_quotes;
    int in_double_quotes;

    in_single_quotes = 0;
    in_double_quotes = 0;
    while(*s)
    {
        if(*s == '\'' && !in_double_quotes)
            in_single_quotes = !in_single_quotes;
        else if(*s == '"' && !in_single_quotes)
            in_double_quotes = !in_double_quotes;
        else if(*s == '\\' && (*(s + 1) == '\'' || *(s + 1) == '"' ))
            s++;
        s++;
    }
    if (in_single_quotes || in_double_quotes)
    {
        //fprintf(stderr, "Error: Unclosed input quotes in \n");
        return 0; // Return 0 to indicate an error
    }
    return 1; // Return 1 if both single and double quotes are balanced
}

static void     parse_single_quotes(const char *s, char *new_value, int *i, int *j)
{
    (*i)++;
    while(s[*i] && s[*i] != '\'')
    {
        new_value[(*j)++] = s[(*i)++];
    }
    if(s[*i] == '\'')
    {
        (*i)++;
    }
}
static void parse_double_quotes(const char *s, char *new_value, int *i, int *j, t_env *env_list)
{
    (*i)++;
    while (s[*i] && s[*i] != '"')
    {
        if (s[*i] == '\\' && (s[*i + 1] == '"' || s[*i + 1] == '\\'))
        {
            (*i)++;
        }
        else if (s[*i] == '$')
        {
            (*i)++;
            char *var_name = malloc(256);
            if (!var_name)
            {
                fprintf(stderr, "Memory allocation error\n");
                exit(EXIT_FAILURE);
            }
            int k = 0;
            while (s[*i] && (isalnum(s[*i]) || s[*i] == '_'))
            {
                var_name[k++] = s[*i];
                (*i)++;
            }
            var_name[k] = '\0';
            char *value = get_env_var(env_list, var_name);
            free(var_name); // Move free here
            if (value)
            {
                while (*value)
                {
                    new_value[(*j)++] = *value++;
                }
            }
            continue;
        }
        new_value[(*j)++] = s[(*i)++];
    }
    if (s[*i] == '"')
    {
        (*i)++;
    }
}

int parse_quotes_in_tokens(t_token *token_list, t_env *env_list)
{
    t_token *current;
    char *s;
    int i;
    int j;
    char *new_value;

    current = token_list;
    while (current)
    {
		if (!current->value)
        {
            fprintf(stderr, "Token value is NULL at parse_quotes_in_tokens\n");
            current = current->next;
            continue; // Skip processing uninitialized tokens
        }
        if (current->type == WORD)
        {
            s = current->value;
            if (!valid_quotes(s))
            {
                //fprintf(stderr, "Error: Unclosed quotes in input: %s\n", s);
                return 0; // Return 0 to indicate an error
            }
            new_value = malloc(strlen(s) + 1);
            if (!new_value)
            {
                fprintf(stderr, "Memory allocation error\n");
                exit(EXIT_FAILURE);
            }

            i = 0;
            j = 0;
            while (s[i])
            {
                if (s[i] == '\'')
                {
                    parse_single_quotes(s, new_value, &i, &j);
                }
                else if (s[i] == '"')
                {
                    parse_double_quotes(s, new_value, &i, &j, env_list);
                }
                else
                {
                    new_value[j++] = s[i++];
                }
            }
            new_value[j] = '\0';

            free(current->value);
            current->value = new_value;
        }
        current = current->next;
    }
    return 1; // Return 1 to indicate success
}
