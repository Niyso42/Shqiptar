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

static void free_cmd_args(char **args) {
  int i;

  if (!args)
    return;
  i = 0;
  while (args[i]) {
    free(args[i]);
    i++;
  }
  free(args);
}

static void free_cmd_heredoc(char **heredoc, int nb_heredoc) {
  int i;

  if (!heredoc)
    return;
  i = 0;
  while (i < nb_heredoc && heredoc[i]) {
    free(heredoc[i]);
    i++;
  }
  free(heredoc);
}

void free_single_cmd(t_cmd *cmd) {
  if (cmd->cmd)
    free(cmd->cmd);
  free_cmd_args(cmd->args);
  if (cmd->infile)
    free(cmd->infile);
  if (cmd->outfile)
    free(cmd->outfile);
  free_cmd_heredoc(cmd->heredoc, cmd->nb_heredoc);
  free(cmd);
}

void free_cmd(t_cmd *cmd) {
  t_cmd *tmp;

  while (cmd) {
    tmp = cmd->next;
    free_single_cmd(cmd);
    cmd = tmp;
  }
}

void free_tokens(t_token *token) {
  t_token *tmp;

  while (token) {
    tmp = token->next;
    free(token->content);
    free(token);
    token = tmp;
  }
}

void free_env(t_env *env) {
  int i;

  i = 0;
  if (!env || !env->env)
    return;
  while (env->env[i]) {
    free(env->env[i]);
    i++;
  }
  free(env->env);
}

void free_data(t_data *data) {
  if (!data)
    return;
  if (data->exit) {
    if (data->exit->exit)
      free(data->exit->exit);
    free(data->exit);
  }
  if (data->env) {
    free_env(data->env);
    free(data->env);
  }
  free(data);
}
