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

int		is_redirect_inf(char *command)
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
		if (command[i] == '<' && is_even(quote.singl) == TRUE &&
				is_even(quote.doubl) == TRUE &&
				(i == 0 || (i >= 1 && command[i - 1] != '\\')))
			return (TRUE);
		i++;
	}
	return (FALSE);
}

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

int		thread_inf(int fd, char *command)
{
	int		fdp[2];
	pid_t	child;
	int		j;
	char	*str;

	j = 1;
	str = NULL;
	pipe(fdp);
	if ((child = fork()) == -1)
		return (-1);
	if (child == 0)
	{
		dup2(fdp[1], 1);
		close(fdp[0]);
		close(fdp[1]);
		while (j == 1)
		{
			j = get_next_line(fd, &str);
			if (j != 0)
			{
				str = ft_charjoin(str, '\n');
				ft_putstr_fd(str, 1);
			}
			free(str);
		}
		exit(0);
	}
	wait(&child);
	if ((child = fork()) == -1)
		return (-1);
	if (child == 0)
	{
		dup2(fdp[0], 0);
		close(fdp[0]);
		close(fdp[1]);
		ft_putstr("entering...\n\n\n\n");
		check_exceptions(command, 0, 0);
		ft_putstr("exiting...\n\n\n\n");
		exit(g_quit);
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
			return (0);
		}
	}
	return (thread_inf(fd, command));
}

int		redirect_inf(char *line)
{
	int		i;
	int		j;
	char	*command;
	char	**tab;
	t_quote	quote;

	i = -1;
	j = 0;
	quote.singl = 0;
	quote.doubl = 0;
	if (!(tab = malloc(sizeof(char *) * (redir_inf_count(line) + 1))))
		return (FAILURE);
	if (!(command = malloc(sizeof(char) * (ft_strlen(line) + 1))))
		return (FAILURE);
	command[0] = '\0';
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
		command = ft_charjoin(command, line[i]);
		if (line[i] == 0)
			break ;
	}
	tab[j] = 0;
	i = exec_inf(tab, command);
	free_tab(tab);
	free(command);
	return (i);
}
