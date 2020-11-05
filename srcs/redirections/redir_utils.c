/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 09:04:30 by mmaj              #+#    #+#             */
/*   Updated: 2020/10/09 17:05:59 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_double_redirect(char *command)
{
	int		i;
	t_quote	quote;

	quote.singl = 0;
	quote.doubl = 0;
	i = 0;
	while (command[i])
	{
		if (command[i] == '\'')
			quote.singl++;
		if (command[i] == '\"')
			quote.doubl++;
		if (command[i] == '>' && command[i + 1] == '>' &&
		is_even(quote.singl) == TRUE && is_even(quote.doubl) == TRUE &&
		(i == 0 || (i >= 1 && command[i - 1] != '\\')))
			return (TRUE);
		i++;
	}
	return (FALSE);
}

int		is_single_redirect(char *command, char c)
{
	int		i;
	t_quote	quote;

	quote.singl = 0;
	quote.doubl = 0;
	i = 0;
	while (command[i])
	{
		if (command[i] == '\'')
			quote.singl++;
		if (command[i] == '\"')
			quote.doubl++;
		if (command[i] == c && is_even(quote.singl) == TRUE &&
				is_even(quote.doubl) == TRUE &&
				(i == 0 || (i >= 1 && command[i - 1] != '\\')))
			return (TRUE);
		i++;
	}
	return (FALSE);
}

int		open_fd(char ***arg_tab, int *fd, int j)
{
	char	*tmp;
	int		k;

	k = 0;
	tmp = ft_strtrim((*arg_tab)[j + 1], " ");
	while (tmp[k] != '\0' && tmp[k] != ' ')
		k++;
	if (tmp[k])
	{
		tmp[k] = '\0';
		while (tmp[++k])
			*arg_tab[0] = ft_charjoin((*arg_tab)[0], tmp[k]);
	}
	if (!(fd[j] = open(tmp, O_CREAT | O_RDWR | O_TRUNC, 0666)))
	{
		free(fd);
		free(tmp);
		return (-1);
	}
	j++;
	free(tmp);
	return (j);
}

char	**tab_creator_double(char **arg_tab, char *line, t_quote quote)
{
	int		i;
	int		j;

	i = -1;
	j = 1;
	while (line[++i])
	{
		if (line[i] == '\'')
			quote.singl++;
		if (line[i] == '\"')
			quote.doubl++;
		if (line[i] == '>' && line[i + 1] == '>' &&
		is_even(quote.singl) == TRUE && is_even(quote.doubl) == TRUE &&
		(i == 0 || (i >= 1 && line[i - 1] != '\\')))
			arg_tab[j++] = word_double(line, &i);
		arg_tab[0] = ft_charjoin(arg_tab[0], line[i]);
		if (line[i] == 0)
			break ;
	}
	free(line);
	arg_tab[j] = 0;
	return (arg_tab);
}

char	**tab_creator_inf(char **tab, char *line, t_quote quote, char **command)
{
	int		i;
	int		j;

	i = -1;
	j = 0;
	while (line[++i])
	{
		if (line[i] == '\'')
			quote.singl++;
		if (line[i] == '\"')
			quote.doubl++;
		if (line[i] == '<' && is_even(quote.singl) == TRUE &&
				is_even(quote.doubl) == TRUE &&
				(i == 0 || (i >= 1 && line[i - 1] != '\\')))
			tab[j++] = word_inf(line, &i);
		*command = ft_charjoin(*command, line[i]);
		if (line[i] == 0)
			break ;
	}
	tab[j] = 0;
	return (tab);
}