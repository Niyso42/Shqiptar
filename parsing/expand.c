/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 17:19:07 by mubersan          #+#    #+#             */
/*   Updated: 2025/06/29 16:19:57 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *get_dollar_value(char *prompt, int j)
{
	int i;
	char *temp;
	int len;
	int k;
	
	i = 0;
	len = 0;
	if (prompt[j] == '?')
		return ft_strdup("?");
	while (isalpha(prompt[j + len]) || isdigit(prompt[j + len]) || prompt[j + len] == '_')
		len++;
	temp = malloc(sizeof(char) * (len + 1));
	k = j;
	while(isalpha(prompt[k]) || isdigit(prompt[k]) || prompt[k] == '_')
	{
		temp[i] = prompt[k];
		i++;
		k++;
	}
	temp[i] = '\0';
	return (temp);
}

char *get_env_value(char *name, t_data *data)
{
    int i;
    int len;
    char *str;

    i = 0;
    len = ft_strlen(name);
    if (ft_strcmp(name, "?") == 0)
        return (str = ft_itoa(*data->exit->exit));

    while (data->env->env[i])
    {
        if (ft_strncmp(data->env->env[i], name, len) == 0 && data->env->env[i][len] == '=')
            return ft_strdup(data->env->env[i] + len + 1);
        i++;
    }
    return (NULL);
}
