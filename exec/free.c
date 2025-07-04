/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 22:57:01 by mubersan          #+#    #+#             */
/*   Updated: 2025/06/29 16:17:38 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void free_env(t_env *env)
{
	int i;

	i = 0;
	while(env->env[i])
	{
		free(env->env[i]);
		i++;
	}
	free(env->env);
}

void free_cmd(t_cmd *cmd)
{
	t_cmd *tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->args)
			free(cmd->args);

		free(cmd);
		cmd = tmp;
	}
}

void free_tokens(t_token *token)
{
	t_token *tmp;

	while (token)
	{
		tmp = token->next;
		free(token->content);
		free(token);
		token = tmp;
	}
}

void free_data(t_data *data)
{
    if (!data)
        return;
    if (data->exit)
    {
        if (data->exit->exit)
            free(data->exit->exit);
        free(data->exit);
    }
    if (data->env)
        free(data->env);
    free(data);
}