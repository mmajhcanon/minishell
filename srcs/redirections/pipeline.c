/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 09:04:30 by mmaj              #+#    #+#             */
/*   Updated: 2020/10/22 15:12:47 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	connect_pipe(char **job_tab, t_pipe p, int i)
{
	dup2(p.pipefd[i + i - 2], 0);
	dup2(p.pipefd[i + i + 1], 1);
	close_fd(p.pipefd, p.nb_pipe * 2);
	check_exceptions(job_tab[i], 0, 0);
	exit(g_quit);
}

int		exec_job(char **job_tab, int i, t_pipe p)
{
	if ((p.child[i] = fork()) < 0)
		exit(0);
	if (p.child[0] == 0)
	{
		dup2(p.pipefd[1], 1);
		close_fd(p.pipefd, p.nb_pipe * 2);
		check_exceptions(job_tab[0], 0, 0);
		wait(NULL);
		exit(g_quit);
	}
	if (p.child[p.nb_job - 1] == 0)
	{
		dup2(p.pipefd[p.nb_fd - 2], 0);
		close_fd(p.pipefd, p.nb_pipe * 2);
		check_exceptions(job_tab[p.nb_job - 1], 0, 0);
		exit(g_quit);
	}
	if (p.child[i] == 0)
		connect_pipe(job_tab, p, i);
	return (0);
}

int		init_pipe(char *command, t_pipe *p, char ***job_tab)
{
	p->nb_pipe = count_pipe(command);
	job_tab[0] = ft_split(command, '|');
	p->nb_job = p->nb_pipe + 1;
	p->nb_fd = p->nb_pipe * 2;
	create_pid_tab(p);
	create_pipefd_tab(p);
	return (SUCCESS);
}

int		pipeline(char *command)
{
	int		i;
	t_pipe	p;
	char	**job_tab;
	int		stat;

	i = -1;
	g_pipe = 1;
	init_pipe(command, &p, &job_tab);
	while (++i < p.nb_job)
		exec_job(job_tab, i, p);
	close_fd(p.pipefd, p.nb_pipe * 2);
	i = 0;
	while (i < p.nb_job)
	{
		waitpid(p.child[i], &stat, 0);
		if (WIFEXITED(stat))
			g_quit = WEXITSTATUS(stat);
		i++;
	}
	free(p.child);
	free(p.pipefd);
	free_tab(job_tab);
	return (g_quit);
}
