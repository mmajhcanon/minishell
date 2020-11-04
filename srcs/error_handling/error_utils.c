/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 11:32:35 by mmaj              #+#    #+#             */
/*   Updated: 2020/10/19 15:03:34 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_bad_char(char *line, int i)
{
	int save;

	save = i;
	i++;
	while (line[i] && line[i] == ' ')
		i++;
	if (is_char(line[i], "|;") == TRUE)
		return (FAILURE);
	while (save > 0 && line[--save] == ' ')
		save = save + 0;
	if (save == 0 || is_char(line[i], "|;") == TRUE)
		return (FAILURE);
	return (SUCCESS);
}

int		check_semicolon(char *line, char *quoted_line)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == ';' && quoted_line[i] == '0'
		&& check_bad_char(line, i) == FAILURE)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

int		check_bad_redirection(char *line, int i)
{
	i++;
	if (line[i] == '>' && line[i - 1] == '>')
		i++;
	while (line[i] && line[i] == ' ')
		i++;
	if (line[i] == '\0' || is_char(line[i], "|;<>") == TRUE)
		return (FAILURE);
	return (SUCCESS);
}

int		check_redirection(char *line, char *quoted_line)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (is_char(line[i], "<>") && quoted_line[i] == '0'
		&& check_bad_redirection(line, i) == FAILURE)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

int		check_pipe(char *line, char *quoted_line)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '|' && quoted_line[i] == '0'
		&& check_bad_char(line, i) == FAILURE)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}
