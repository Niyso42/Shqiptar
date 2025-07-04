/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:31:02 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/06/29 16:03:21 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void export_display(t_data *data)
{
    char **sorted;
    int i;

    sorted = ft_strdup_array(data->env->env);
    ft_sort_ascii(sorted);
    i = 0;
    while (sorted[i])
    {
        ft_putstr_fd("declare -x ", 1);
        char *eq = ft_strchr(sorted[i], '=');
        if (eq)
        {
            *eq = '\0';
            ft_putstr_fd(sorted[i], 1);
            ft_putstr_fd("=\"", 1);
            ft_putstr_fd(eq + 1, 1);
            ft_putstr_fd("\"\n", 1);
            *eq = '=';
        }
        else
        {
            ft_putstr_fd(sorted[i], 1);
            write(1, "\n", 1);
        }
        i++;
    }
    free_array(sorted);
}

void export_add_or_update(t_data *data, char *arg)
{
    int i;
    char *var_name;
    size_t len;
    char *equal_pos;

    equal_pos = ft_strchr(arg, '=');
    if (equal_pos)
        len = equal_pos - arg;
    else
        len = ft_strlen(arg);

    var_name = ft_substr(arg, 0, len);
    i = 0;
    while (data->env->env[i])
    {
        if (!ft_strncmp(data->env->env[i], var_name, len) && data->env->env[i][len] == '=')
        {
            free(data->env->env[i]);
            data->env->env[i] = ft_strdup(arg);
            free(var_name);
            return;
        }
        i++;
    }
    if (equal_pos)
        data->env->env = ft_add_to_array(data->env->env, arg);
    else
    {
        char *new_var = ft_strjoin(arg, "=");
        data->env->env = ft_add_to_array(data->env->env, new_var);
        free(new_var);
    }
    free(var_name);
}

int ft_export(t_data *data, char **args)
{
    int i;

    if (!args[1])
        export_display(data);
    else
    {
        i = 1;
        while (args[i])
        {
            if (is_valid_export_identifier(args[i]))
                export_add_or_update(data, args[i]);
            else
                ft_puterror("minishell: export: `", args[i], "`: not a valid identifier");
            i++;
        }
    }
    return (0);
}

int	is_valid_export_identifier(const char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

