/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <hcanon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 12:07:09 by user42            #+#    #+#             */
/*   Updated: 2020/09/10 11:28:07 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		exit_error2(void)
{
	ft_putstr_fd("msh: exit: too many arguments\n", 2);
	g_quit = 1;
	return (-1);
}

void	exit_error(char **arg_tab)
{
	ft_putstr_fd("msh: ", 2);
	ft_putstr_fd(arg_tab[1], 2);
	ft_putstr_fd(" : numeric argument only\n", 2);
}

int		ft_exit(char **arg_tab)
{
	int		i;

	i = -1;
	if (g_pipe == 0)
		ft_putstr("exit\n");
	while (arg_tab[1] && arg_tab[1][++i])
		if (ft_isdigit(arg_tab[1][i]) == FALSE &&
		arg_tab[1][i] != '-' && arg_tab[1][i] != '+')
			break ;
	if (arg_tab[1] && arg_tab[1][i])
	{
		exit_error(arg_tab);
		g_quit = 1;
		return (-1);
	}
	if (arg_tab[1] != 0 && arg_tab[2] != 0)
		return (exit_error2());
	if (arg_tab[1])
		g_quit = ft_atoi(arg_tab[1]);
	else
		g_quit = 0;
	return (-1);
}
