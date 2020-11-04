/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 09:04:30 by mmaj              #+#    #+#             */
/*   Updated: 2020/10/22 15:12:47 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_char_no_quot(char *command, char c)
{
	int		i;
	char	*quoted_line;

	i = 0;
	(void)c;
	quoted_line = malloc(sizeof(char) * (ft_strlen(command) + 1));
	count_quote(command, &quoted_line);
	while (command[i])
	{
		if (is_char(command[i], "|") == TRUE && quoted_line[i] == '0')
		{
			free(quoted_line);
			return (TRUE);
		}
		i++;
	}
	free(quoted_line);
	return (FALSE);
}

int		count_pipe(char *command)
{
	int		i;
	int		count;
	char	*quoted_line;

	i = 0;
	count = 0;
	quoted_line = malloc(sizeof(char) * (ft_strlen(command) + 1));
	count_quote(command, &quoted_line);
	while (command[i])
	{
		if (command[i] == '|' && quoted_line[i] == '0')
			count++;
		i++;
	}
	free(quoted_line);
	return (count);
}

int		close_fd(int *pipefd, int nb_fd)
{
	int		i;

	i = 0;
	while (i < nb_fd)
	{
		close(pipefd[i]);
		i++;
	}
	return (SUCCESS);
}

int		create_pid_tab(t_pipe *p)
{
	int	i;

	i = 0;
	p->child = malloc(sizeof(pid_t) * p->nb_job);
	while (i < p->nb_job)
	{
		p->child[i] = -1;
		i++;
	}
	return (SUCCESS);
}

int		create_pipefd_tab(t_pipe *p)
{
	int i;

	i = 0;
	p->pipefd = malloc(sizeof(int) * (p->nb_fd));
	while (i < p->nb_fd)
	{
		pipe(&p->pipefd[i]);
		i = i + 2;
	}
	return (SUCCESS);
}
