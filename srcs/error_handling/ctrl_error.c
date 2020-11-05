/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/04 14:46:52 by user42            #+#    #+#             */
/*   Updated: 2020/09/24 16:12:09 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void		ctrlc_handler(int signum)
{
	if (signum == SIGINT)
	{
		ft_putchar_fd('\n', 2);
		print_prompt(0);
		g_quit = 130;
	}
	if (signum == SIGQUIT)
		ft_putstr_fd("\b\b  \b\b", 2);
}

void		cancel_handler(int signum)
{
	if (signum == SIGINT)
	{
		ft_putchar_fd('\n', 2);
		g_quit = 130;
	}
	else if (signum == SIGQUIT)
	{
		ft_putstr_fd("Quitted: (core dumped)\n", 2);
		g_quit = 131;
	}
}
