# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shkaruna <shkaruna@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/20 11:39:47 by shkaruna          #+#    #+#              #
#    Updated: 2024/12/15 22:21:04 by shkaruna         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

CPPFLAGS = -I/usr/local/opt/readline/include  # Update this with your correct path if needed

# Link to Readline library
LDFLAGS = -L/usr/local/opt/readline/lib 

NAME = minishell

SRCS = main.c Envp/env_vars.c Execution/Builtins/builtins.c Execution/Builtins/ft_cd.c\
Execution/Builtins/ft_echo.c Execution/Builtins/ft_env.c Execution/Builtins/ft_export.c\
Execution/Builtins/ft_pwd.c Execution/Builtins/ft_unset.c Lexing/ft_handle_quotes.c \
Lexing/ft_is_op.c Lexing/ft_is_quote.c Lexing/ft_is_word_ws.c Lexing/lexing.c \
Utils/utils.c Utils/utils1.c Execution/Exec/Exec.c Execution/Exec/ft_external_commands.c \
Utils/ft_split.c Execution/Builtins/ft_exit.c Execution/Exec/ft_execute_pipeline.c \
Execution/Exec/Signals/ft_signals.c Parsing/ft_parse_pipeline.c \
Parsing/ft_parsing.c Execution/Exec/ft_handle_redirection.c Utils/get_next_line.c \
Execution/Builtins/builtins_utils.c\

OBJS = $(SRCS:.c=.o)

MINISHELL_H := minishell.h

all: $(MINISHELL_H) $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(NAME) -lreadline

clean:
	$(RM) $(OBJS) *.gch

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

