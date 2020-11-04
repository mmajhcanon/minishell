/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 14:16:45 by mmaj              #+#    #+#             */
/*   Updated: 2020/10/21 15:10:12 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	swap_str(int i, char **tabstr)
{
	char	*tmp;

	tmp = tabstr[i + 1];
	tabstr[i + 1] = tabstr[i];
	tabstr[i] = tmp;
}

char	**sort_tabstr(char **tabstr)
{
	int		i;

	i = 0;
	while (tabstr[i + 1] != NULL)
	{
		if (ft_strcmp(tabstr[i], tabstr[i + 1]) > 0)
		{
			swap_str(i, tabstr);
			i = -1;
		}
		i++;
	}
	return (tabstr);
}

int		is_equal(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (TRUE);
		i++;
	}
	return (FALSE);
}

int		is_bad_char(char c)
{
	if (c == '\"' || c == '\\' || c == '\'')
		return (TRUE);
	return (FALSE);
}

void	bad_identifier(char *arg, char *var)
{
	ft_putstr_fd("msh : export : \'", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("\': not a valid identifier\n", 2);
	free(var);
}
