/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_inf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 09:04:30 by mmaj              #+#    #+#             */
/*   Updated: 2020/10/09 17:05:59 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		redir_inf_count(char *line)
{
	int		i;
	int		j;

	j = 0;
	i = -1;
	while (line[++i])
		if (is_char(line[i], "<") == TRUE)
			j++;
	return (j);
}

char	*word_inf(char *line, int *i)
{
	char	*str;
	int		j;

	j = -1;
	if (!(str = malloc(sizeof(char) * ft_strlen(line + *i) + 1)))
		return (NULL);
	(*i)++;
	while (line[*i] && line[*i] == ' ')
		(*i)++;
	while (line[*i] && line[*i] != ' ')
		str[++j] = line[(*i)++];
	str[++j] = 0;
	return (str);
}

int		thread_inf(int in, char *command)
{
	pid_t	child;

	if ((child = fork()) == -1)
		return (-1);
	if (child == 0)
	{
		dup2(in, 0);
		close(in);
		check_exceptions(command, 0, 0);
		exit(0);
	}
	wait(&child);
	return (1);
}

int		exec_inf(char **tab, char *command)
{
	int		i;
	int		fd;

	i = -1;
	while (tab[++i])
	{
		if (fd)
			close(fd);
		fd = open(tab[i], O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("msh: no such file or directory: ", 2);
			ft_putstr_fd(tab[i], 2);
			ft_putchar_fd('\n', 2);
			g_quit = 1;
			return (0);
		}
	}
	return (thread_inf(fd, command));
}

int		redirect_inf(char *line)
{
	int		i;
	char	*command;
	char	**tab;
	t_quote	quote;

	i = -1;
	quote.singl = 0;
	quote.doubl = 0;
	if (!(tab = malloc(sizeof(char *) * (redir_inf_count(line) + 1))))
		return (FAILURE);
	if (!(command = malloc(sizeof(char) * (ft_strlen(line) + 1))))
		return (FAILURE);
	command[0] = '\0';
	tab = tab_creator_inf(tab, line, quote, &command);
	i = exec_inf(tab, command);
	free_tab(tab);
	free(command);
	return (i);
}
