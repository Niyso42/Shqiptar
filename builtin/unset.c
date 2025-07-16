/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:13:49 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/06/29 16:04:40 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_valid_identifier(const char *name)
{
	int	i;

	if (!name || !ft_isalpha(name[0]))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	remove_env_var(t_data *data, char *name)
{
	int		j;
	size_t	len;

	j = 0;
	len = ft_strlen(name);
	while (data->env->env[j])
	{
		if (ft_strncmp(data->env->env[j], name, len) == 0 
			&& data->env->env[j][len] == '=')
		{
			free(data->env->env[j]);
			while (data->env->env[j])
			{
				data->env->env[j] = data->env->env[j + 1];
				j++;
			}
			break ;
		}
		j++;
	}
}

void	ft_unset(char **args, t_data *data)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("unset: not a valid identifier\n", 2);
			*data->exit->exit = 1;
		}
		else
		{
			remove_env_var(data, args[i]);
			*data->exit->exit = 0;
		}
		i++;
	}
}
