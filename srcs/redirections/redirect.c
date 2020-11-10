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

void	free_redirect_sup(char **arg_tab, int *fd)
{
	int		j;

	free_tab(arg_tab);
	j = -1;
	while (fd && fd[++j])
		close(fd[j]);
	if (fd)
		free(fd);
}

int		threading(char **arg_tab, int *fd, int j, int i)
{
	pid_t	child;

	child = fork();
	if (child == 0)
	{
		if (dup2(fd[j], 1) != FAILURE)
			check_exceptions(arg_tab[0], i, 0);
		return (FAILURE);
	}
	else if (child < 0)
	{
		ft_putstr_fd("msh: failed to create a new thread\n", 2);
		g_quit = 126;
		return (0);
	}
	wait(&child);
	return (1);
}

int		*initialize_fd(char ***arg_tab, int i, int j)
{
	int		*fd;

	while ((*arg_tab)[i])
		i++;
	if (!(fd = (int *)malloc(sizeof(int) * i)))
		return (NULL);
	while (i-- > 1)
		if ((j = open_fd(arg_tab, fd, j)) == -1)
			return (NULL);
	fd[j] = 0;
	return (fd);
}

char	**copy_string(char *str)
{
	char	*line;
	char	**arg_tab;
	int		i;

	i = -1;
	line = ft_strdup(" ");
	while (str[++i])
		line = ft_charjoin(line, str[i]);
	arg_tab = ft_split(line, '>');
	free(line);
	change_arg(arg_tab, 0, 0);
	return (arg_tab);
}

int		redirect_sup(char *str, int type)
{
	char	**arg_tab;
	int		j;
	int		*fd;
	int		i;

	j = 0;
	i = 1;
	arg_tab = copy_string(str);
	if ((fd = initialize_fd(&arg_tab, 0, 0)) == NULL)
	{
		ft_putstr_fd("msh: ambiguous redirection\n", 2);
		free_tab(arg_tab);
		g_quit = 1;
		return (FALSE);
	}
	while (fd[j])
		j++;
	if (fd[0] == 0)
		i = check_exceptions(str, 1, 0);
	else if (type == 0)
		i = threading(arg_tab, fd, j - 1, 2);
	else
		i = check_exceptions(arg_tab[0], 2, 0);
	free_redirect_sup(arg_tab, fd);
	return (i);
}
