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

int		is_export(char **env, char *var)
{
	int		i;
	int		j;

	j = 0;
	while (env[j])
	{
		i = 0;
		while (env[j][i] && env[j][i] == var[i] && env[j][i] != '=')
			i++;
		if (env[j][i] == '\0' || env[j][i] == '=')
			return (j);
		j++;
	}
	return (FAILURE);
}

char	***ft_replace_var(char ***env, char *var)
{
	int		j;
	int		i;

	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	if (var[i] == '=')
	{
		j = is_export(env[0], var);
		free(env[0][j]);
		env[0][j] = ft_strdup(var);
	}
	return (env);
}

int		get_new_var(char *arg, char **var)
{
	int		i;

	i = 0;
	if (ft_isdigit(arg[i]) || is_char(arg[i], "\"\\\';") || arg[i] == '\0')
	{
		bad_identifier(arg, var[0]);
		return (FAILURE);
	}
	while (arg[i] && is_char(arg[i], "\"\\\';") == FALSE)
	{
		var[0][i] = arg[i];
		i++;
	}
	if (is_char(arg[i], "\"\\\';") == TRUE)
	{
		bad_identifier(arg, var[0]);
		return (FAILURE);
	}
	var[0][i] = '\0';
	return (SUCCESS);
}

int		add_var(char *arg, char ***env)
{
	int		i;
	char	*var;
	char	**save;

	i = 0;
	save = env[0];
	while (arg[i])
		i++;
	var = malloc(sizeof(var) * (i + 1));
	if (get_new_var(arg, &var) == FAILURE)
		return (FAILURE);
	if (is_export(env[0], var) != FAILURE)
	{
		ft_replace_var(env, var);
	}
	else
	{
		env[0] = ft_add_tabstrdup(env[0], var);
		free_tab(save);
	}
	free(var);
	return (SUCCESS);
}

int		ft_export(char **arg_tab, char ***env)
{
	int		i;
	int		ret;
	char	**alloc_env;

	i = 1;
	ret = 2;
	alloc_env = ft_tabstrdup(env[0]);
	if (arg_tab[i] == NULL)
		print_exported_var(alloc_env);
	else
	{
		while (arg_tab[i] != NULL)
		{
			if (add_var(arg_tab[i], env) == FAILURE)
			{
				g_quit = 1;
				ret = 1;
			}
			i++;
		}
	}
	free_tab(alloc_env);
	return (ret);
}
