/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/04 14:46:52 by user42            #+#    #+#             */
/*   Updated: 2020/10/15 14:16:43 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			special_var(char **proper_arg)
{
	char	*tmp;
	char	*tmp2;

	tmp2 = ft_itoa(g_quit);
	tmp = ft_strjoin(*proper_arg, tmp2);
	free(tmp2);
	free(*proper_arg);
	*proper_arg = tmp;
	return (SUCCESS);
}
