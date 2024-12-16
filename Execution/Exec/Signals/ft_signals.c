/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shkaruna <shkaruna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 12:21:40 by shkaruna          #+#    #+#             */
/*   Updated: 2024/12/15 23:30:29 by shkaruna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

volatile sig_atomic_t	g_command_running = 0;

void	signal_reset_prompt(int signo)
{
	(void)signo;
	write(STDOUT_FILENO, "\n", 2);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ignore_sigquit(void)
{
	signal(SIGQUIT, SIG_IGN);
}

void	set_signals_interactive(void)
{
	ignore_sigquit();
	signal(SIGINT, signal_reset_prompt);
}

void	signal_print_newline(int signal)
{
	(void)signal;
	rl_on_new_line();
}

void	set_signals_noninteractive(void)
{
	signal(SIGINT, signal_print_newline);
	ignore_sigquit();
}
