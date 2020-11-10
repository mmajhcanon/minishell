/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 09:04:30 by mmaj              #+#    #+#             */
/*   Updated: 2020/10/09 17:05:59 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		redir_double_count(char *line)
{
	int		i;
	int		j;

	j = 0;
	i = -1;
	while (line[++i])
		if (is_char(line[i], ">") == TRUE && is_char(line[i + 1], ">") == TRUE)
			j++;
	return (j);
}

int		*initialize_fdd(char ***arg_tab, int i, int j)
{
	int		*fd;
	char	*tmp;

	while ((*arg_tab)[i])
		i++;
	if (!(fd = (int *)malloc(sizeof(int) * i)))
		return (NULL);
	while (i-- > 1)
	{
		tmp = (*arg_tab)[j + 1];
		if (tmp[0] == '\0' ||
			!(fd[j] = open(tmp, O_CREAT | O_RDWR | O_APPEND, 0666)))
		{
			free(fd);
			return (NULL);
		}
		j++;
		if (i - 1 > 1)
			close(fd[j]);
	}
	fd[j] = 0;
	return (fd);
}

char	*word_double(char *line, int *i)
{
	char	*str;
	int		j;

	j = -1;
	if (!(str = malloc(sizeof(char) * ft_strlen(line + *i) + 1)))
		return (NULL);
	(*i)++;
	(*i)++;
	while (line[*i] && line[*i] == ' ')
		(*i)++;
	while (line[*i] && line[*i] != ' ')
		str[++j] = line[(*i)++];
	str[++j] = 0;
	return (str);
}

char	**copy_stringd(char *str)
{
	char	*line;
	char	**arg_tab;
	int		i;
	t_quote	quote;

	i = -1;
	quote.doubl = 0;
	quote.singl = 0;
	line = ft_strdup(" ");
	if (!(arg_tab = malloc(sizeof(char *) * (redir_double_count(str) + 2))))
		return (NULL);
	arg_tab[0] = ft_strdup("");
	while (str[++i])
		line = ft_charjoin(line, str[i]);
	arg_tab = tab_creator_double(arg_tab, line, quote);
	return (arg_tab);
}

int		double_redirect(char *str)
{
	char	**arg_tab;
	int		*fd;
	int		i;

	i = 0;
	arg_tab = copy_stringd(str);
	if ((fd = initialize_fdd(&arg_tab, 0, 0)) == NULL)
	{
		ft_putstr_fd("msh: ambiguous redirection\n", 2);
		free_tab(arg_tab);
		g_quit = 1;
		return (FALSE);
	}
	while (fd[i])
		i++;
	i = threading(arg_tab, fd, i - 1, 1);
	free_redirect_sup(arg_tab, fd);
	return (i);
}
