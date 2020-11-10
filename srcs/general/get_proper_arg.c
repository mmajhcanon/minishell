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

void		replace_arg_cond(t_quote *q, int *i, char *arg, char **proper_arg)
{
	if (arg[i[0]] == '\\' && is_even(q->doubl) && is_even(q->singl))
	{
		i[0]++;
		proper_arg[0] = ft_charjoin(proper_arg[0], arg[i[0]]);
	}
	else if (arg[i[0]] == '\\'
	&& !is_even(q->doubl) && is_char(arg[i[0] + 1], "\"$\\"))
	{
		i[0]++;
		proper_arg[0] = ft_charjoin(proper_arg[0], arg[i[0]]);
	}
	else if (arg[i[0]] == '\'' && is_even(q->doubl))
		q->singl++;
	else if (arg[i[0]] == '\"' && is_even(q->singl))
		q->doubl++;
	else if (arg[i[0]] == '$' && is_even(q->singl) && arg[(*i) + 1])
	{
		get_ve_value(proper_arg, arg, i[0] + 1, q);
		i[0] = pass_ve(arg, i[0]) - 1;
	}
	else
		proper_arg[0] = ft_charjoin(proper_arg[0], arg[i[0]]);
}

char		*replace_arg(char *arg)
{
	int		i;
	t_quote	q;
	char	*proper_arg;
	char	*save;

	i = -1;
	q.doubl = 0;
	q.singl = 0;
	if (!(proper_arg = malloc(sizeof(char) * 1)))
		return (NULL);
	proper_arg[0] = '\0';
	if (arg[0] == '~' && (arg[1] == '\0' || arg[1] == '/'))
	{
		save = proper_arg;
		proper_arg = ft_strjoin(proper_arg, ft_search("HOME"));
		free(save);
		i++;
	}
	while (arg[++i])
		replace_arg_cond(&q, &i, arg, &proper_arg);
	return (proper_arg);
}

int			arrange_tab(char ***arg_tab, int j)
{
	int		i;
	char	**new_arg_tab;

	i = 0;
	while (arg_tab[0][i] != NULL)
		i++;
	if (!(new_arg_tab = malloc(sizeof(new_arg_tab) * (i))))
		return (FAILURE);
	i = 0;
	while (arg_tab[0][i] && i < j)
	{
		new_arg_tab[i] = ft_strdup(arg_tab[0][i]);
		i++;
	}
	j++;
	while (arg_tab[0][j])
	{
		new_arg_tab[i] = ft_strdup(arg_tab[0][j]);
		i++;
		j++;
	}
	new_arg_tab[i] = NULL;
	free_tab(arg_tab[0]);
	arg_tab[0] = new_arg_tab;
	return (SUCCESS);
}

int			is_wrong_ve(char *job)
{
	int		i;

	i = 0;
	if (job[i] == '$' && job[i + 1] == '\0')
		return (FALSE);
	if (job[i] != '$' && !ft_isalpha(job[i + 1]))
		return (FALSE);
	i++;
	while (job[i] && ft_isalnum(job[i]))
		i++;
	if (job[i] == '\0' && ft_search(&job[1]) == NULL)
		return (TRUE);
	return (FALSE);
}

char		**get_proper_arg(char **arg_tab, int i)
{
	int		j;
	char	*proper_arg;

	j = 0;
	while (arg_tab[j])
	{
		if (is_replace(arg_tab[j]) == TRUE)
		{
			if (is_wrong_ve(arg_tab[j]) && i == 0)
				arrange_tab(&arg_tab, j--);
			else
			{
				proper_arg = replace_arg(arg_tab[j]);
				free(arg_tab[j]);
				arg_tab[j] = proper_arg;
			}
		}
		j++;
	}
	return (arg_tab);
}
