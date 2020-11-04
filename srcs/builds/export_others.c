/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaj <mmaj@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/18 14:16:45 by mmaj              #+#    #+#             */
/*   Updated: 2020/10/21 15:10:12 by mmaj             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**put_between_quote(char **export_env)
{
	int		i;
	char	*newline;

	i = 0;
	while (export_env[i] != NULL)
	{
		if (is_equal(export_env[i]) == TRUE)
		{
			newline = put_quote(export_env[i]);
			free(export_env[i]);
			export_env[i] = newline;
		}
		i++;
	}
	return (export_env);
}

char	*put_quote(char *str)
{
	int		j;
	char	*newline;

	j = 0;
	newline = malloc(sizeof(newline) * (ft_strlen(str) + 3));
	while (str[j] && str[j] != '=')
	{
		newline[j] = str[j];
		j++;
	}
	newline[j++] = '=';
	newline[j++] = '\"';
	while (str[j - 1] != '\0')
	{
		newline[j] = str[j - 1];
		j++;
	}
	newline[j] = '\"';
	newline[++j] = '\0';
	return (newline);
}

int		print_exported_var(char **alloc_env)
{
	int		i;

	i = 0;
	alloc_env = sort_tabstr(alloc_env);
	alloc_env = put_between_quote(alloc_env);
	while (alloc_env[i] != NULL)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(alloc_env[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	return (1);
}

char	**ft_add_tabstrdup(char **tabstr, char *var)
{
	int		j;
	char	**new_tabstr;

	j = 0;
	while (tabstr[j] != NULL)
		j++;
	if (!(new_tabstr = malloc(sizeof(tabstr) * (j + 2))))
		return (NULL);
	j = 0;
	while (tabstr[j])
	{
		new_tabstr[j] = ft_strdup(tabstr[j]);
		j++;
	}
	new_tabstr[j] = ft_strdup(var);
	new_tabstr[++j] = NULL;
	return (new_tabstr);
}
