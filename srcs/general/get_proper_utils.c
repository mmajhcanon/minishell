/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_proper_arg.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 16:27:37 by mmaj              #+#    #+#             */
/*   Updated: 2020/10/21 15:47:02 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		change_arg(char **arg_tab, int j, int k)
{
	char	**tab;
	char	*str;
	int		i;

	i = j;
	while (arg_tab[++i] != 0)
	{
		str = ft_strdup("");
		tab = ft_split(arg_tab[i], ' ');
		tab = get_proper_arg(tab, 1);
		j = -1;
		while (tab[++j] != 0)
		{
			k = -1;
			if (j == 0)
				while (tab[j][++k] != '\0')
					str = ft_charjoin(str, tab[j][k]);
			else if ((arg_tab[0] = ft_charjoin(arg_tab[0], ' ')))
				while (tab[j][++k] != '\0')
					arg_tab[0] = ft_charjoin(arg_tab[0], tab[j][k]);
		}
		free(arg_tab[i]);
		arg_tab[i] = str;
		free_tab(tab);
	}
}

int			ve_value_free(char *str_name, char **prop_arg)
{
	char	*tmp;
	char	*save;

	if (ft_search(str_name) == NULL)
	{
		free(str_name);
		return (FAILURE);
	}
	tmp = ft_strjoin(*prop_arg, ft_search(str_name));
	free(str_name);
	save = *prop_arg;
	*prop_arg = tmp;
	free(save);
	return (SUCCESS);
}

int			get_ve_value(char **proper_arg, char *arg, int i, t_quote *q)
{
	char	*str_name;
	int		j;

	j = 0;
	if (arg[i] == '?')
		return (special_var(proper_arg));
	if (arg[i] == '\'' && is_even(q->doubl))
		return (FAILURE);
	if (ft_isalnum(arg[i]) == FALSE)
	{
		proper_arg[0] = ft_charjoin(proper_arg[0], '$');
		return (SUCCESS);
	}
	if (!(str_name = malloc(sizeof(char) * ft_strlen(arg) + 1)))
		return (FAILURE);
	while (arg[i] && is_char(arg[i], "\\\'\"$ ") == FALSE)
		str_name[j++] = arg[i++];
	str_name[j] = '\0';
	if (ve_value_free(str_name, proper_arg) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int			pass_ve(char *arg, int i)
{
	i++;
	if (is_char(arg[i], "0123456789") == TRUE)
		return (++i);
	while (arg[i] && is_char(arg[i], "\\\'\"$/ ") == FALSE)
		i++;
	return (i);
}
