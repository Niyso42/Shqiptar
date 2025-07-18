/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:40:03 by mubersan          #+#    #+#             */
/*   Updated: 2025/06/29 15:53:37 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strjoin_three(const char *s1, const char *s2, const char *s3)
{
	char	*result;
	size_t	len1;
	size_t	len2;
	size_t	len3;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	result = malloc(len1 + len2 + len3 + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, len1);
	ft_memcpy(result + len1, s2, len2);
	ft_memcpy(result + len1 + len2, s3, len3);
	result[len1 + len2 + len3] = '\0';
	return (result);
}

static int	set_env_var(t_data *data, const char *key, const char *value)
{
	int		i;
	size_t	len;
	char	*new_var;

	if (!data || !key || !value)
		return (1);
	len = ft_strlen(key);
	new_var = ft_strjoin_three(key, "=", value);
	if (!new_var)
		return (1);
	i = 0;
	while (data->env->env[i])
	{
		if (ft_strncmp(data->env->env[i], key, len) == 0 
			&& data->env->env[i][len] == '=')
		{
			free(data->env->env[i]);
			data->env->env[i] = new_var;
			return (0);
		}
		i++;
	}
	free(new_var);
	return (1);
}

static int	update_pwd(t_data *data, char *oldpwd)
{
	char	cwd[BUFFER_SIZE];

	if (!getcwd(cwd, sizeof(cwd)))
		return (perror("cd"), 1);
	if (set_env_var(data, "OLDPWD", oldpwd) != 0)
		return (1);
	if (set_env_var(data, "PWD", cwd) != 0)
		return (1);
	return (0);
}

static int	get_current_dir(char *cwd)
{
	if (!getcwd(cwd, BUFFER_SIZE))
	{
		perror("cd");
		return (1);
	}
	return (0);
}

static char	*backup_current_dir(void)
{
	char	cwd[BUFFER_SIZE];
	char	*oldpwd;

	if (get_current_dir(cwd) != 0)
		return (NULL);
	oldpwd = ft_strdup(cwd);
	if (!oldpwd)
		perror("cd: malloc");
	return (oldpwd);
}

static int	change_directory(char *path)
{
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

static int	validate_cd_args(char **args, t_data *data)
{
	if (!args || !args[1])
	{
		ft_putstr_fd("cd: missing path argument\n", 2);
		*data->exit->exit = 1;
		return (0);
	}
	return (1);
}

static int	handle_cd_operations(char **args, t_data *data, char *oldpwd)
{
	if (change_directory(args[1]) != 0)
	{
		*data->exit->exit = 1;
		return (0);
	}
	if (update_pwd(data, oldpwd) != 0)
	{
		*data->exit->exit = 1;
		return (0);
	}
	*data->exit->exit = 0;
	return (1);
}

void	ft_cd(char **args, t_data *data)
{
	char	*oldpwd;

	if (!validate_cd_args(args, data))
		return ;
	oldpwd = backup_current_dir();
	if (!oldpwd)
	{
		*data->exit->exit = 1;
		return ;
	}
	handle_cd_operations(args, data, oldpwd);
	free(oldpwd);
}
