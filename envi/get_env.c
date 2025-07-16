/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 18:54:11 by mubersan          #+#    #+#             */
/*   Updated: 2025/06/29 22:00:34 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	copy_env(t_data *data, char **envp)
{
	int	i;
	int	j;

	if (!data || !envp)
		return ;
	i = 0;
	while (envp[i])
		i++;
	data->env->env = (char **)malloc(sizeof(char *) * (i + 1));
	if (!data->env->env)
		return ;
	j = 0;
	while (j < i)
	{
		data->env->env[j] = ft_strdup(envp[j]);
		if (!data->env->env[j])
		{
			while (j-- > 0)
				free(data->env->env[j]);
			free(data->env->env);
			data->env->env = NULL;
			return ;
		}
		j++;
	}
	data->env->env[j] = NULL;
}

int	error_handling(int err, t_data *data)
{
	if (data)
		free_data(data); 

	if (err == 6)
		perror("Error creating pipe");
	else if (err == 3)
	{
		perror("Command not found");
		exit(127);
	}
	exit(EXIT_FAILURE);
}

void	free_tab(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

static char	*find_path_env(t_data *data)
{
	int	i;

	i = 0;
	while (data->env->env[i] && ft_strncmp(data->env->env[i], "PATH=", 5) != 0)
		i++;
	if (!data->env->env[i])
		return (NULL);
	return (data->env->env[i] + 5);
}

static char	*build_full_path(char *dir, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(dir, "/");
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	return (full_path);
}

char	*get_path(char *cmd, t_data *data, char **argv)
{
	int		i;
	char	**path;
	char	*good_path;
	char	*path_env;

	(void)argv;
	path_env = find_path_env(data);
	if (!path_env)
		return (NULL);
	path = ft_split(path_env, ':');
	i = 0;
	while (path[i])
	{
		good_path = build_full_path(path[i], cmd);
		if (access(good_path, X_OK) == 0)
			return (free_tab(path), good_path);
		free(good_path);
		i++;
	}
	free_tab(path);
	return (NULL);
}

