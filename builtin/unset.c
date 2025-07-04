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

int is_valid_identifier(const char *name)
{
    if (!name || !ft_isalpha(name[0]))
        return (0);
    for (int i = 1; name[i]; i++)
    {
        if (!ft_isalnum(name[i]) && name[i] != '_')
            return (0);
    }
    return (1);
}

void ft_unset(char **args, t_data *data)
{
    int i = 1; // args[0] == "unset"
    size_t len;

    while (args[i])
    {
        if (!is_valid_identifier(args[i]))
        {
            ft_putstr_fd("unset: not a valid identifier\n", 2);
            *data->exit->exit = 1;
        }
        else
        {
            int j = 0;
            len = ft_strlen(args[i]);
            while (data->env->env[j])
            {
                if (ft_strncmp(data->env->env[j], args[i], len) == 0 && data->env->env[j][len] == '=')
                {
                    free(data->env->env[j]);
                    while (data->env->env[j])
                    {
                        data->env->env[j] = data->env->env[j + 1];
                        j++;
                    }
                    break;
                }
                j++;
            }
            *data->exit->exit = 0;
        }
        i++;
    }
}
