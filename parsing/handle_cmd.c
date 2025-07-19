/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 23:17:09 by mubersan          #+#    #+#             */
/*   Updated: 2025/06/29 17:50:40 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void init_first_arg(t_cmd *cmd, t_token *token) {
  cmd->args = malloc(sizeof(char *) * 2);
  if (!cmd->args)
    return;
  cmd->args[0] = ft_strdup(token->content);
  cmd->args[1] = NULL;
}

static void add_arg_to_existing(t_cmd *cmd, t_token *token) {
  int i;
  char **tmp;
  int j;

  i = 0;
  while (cmd->args[i])
    i++;
  tmp = malloc(sizeof(char *) * (i + 2));
  if (!tmp)
    return;
  j = 0;
  while (j < i) {
    tmp[j] = cmd->args[j];
    j++;
  }
  tmp[j] = ft_strdup(token->content);
  tmp[j + 1] = NULL;
  free(cmd->args);
  cmd->args = tmp;
}

void handle_word_cmd(t_cmd *cmd, t_token *token) {
  if (cmd->cmd == NULL)
    cmd->cmd = ft_strdup(token->content);
  else {
    if (cmd->args == NULL)
      init_first_arg(cmd, token);
    else
      add_arg_to_existing(cmd, token);
  }
}

t_cmd *init_struct(void) {
  t_cmd *new_cmd;

  new_cmd = malloc(sizeof(t_cmd));
  if (!new_cmd)
    return (NULL);
  new_cmd->cmd = NULL;
  new_cmd->args = NULL;
  new_cmd->infile = NULL;
  new_cmd->outfile = NULL;
  new_cmd->append = 0;
  new_cmd->heredoc = NULL;
  new_cmd->nb_heredoc = 0;
  new_cmd->heredoc_fd = 0;
  new_cmd->next = NULL;
  return (new_cmd);
}

static int init_exit_data(t_data *data) {
  data->exit = malloc(sizeof(t_exit));
  if (!data->exit)
    return (0);
  data->exit->exit = malloc(sizeof(int));
  if (!data->exit->exit) {
    free(data->exit);
    return (0);
  }
  *data->exit->exit = 0;
  return (1);
}

static int init_env_data(t_data *data, char **envp) {
  data->env = malloc(sizeof(t_env));
  if (!data->env)
    return (0);
  copy_env(data, envp);
  if (!data->env->env) {
    free(data->env);
    return (0);
  }
  return (1);
}

t_data *init_data(char **envp) {
  t_data *data;

  data = malloc(sizeof(t_data));
  if (!data)
    return (NULL);
  if (!init_exit_data(data))
    return (free(data), NULL);
  if (!init_env_data(data, envp)) {
    free(data->exit->exit);
    free(data->exit);
    return (free(data), NULL);
  }
  data->token = NULL;
  data->cmd = NULL;
  data->should_exit = 0;
  return (data);
}

void handle_pipe_cmd(t_cmd **current_cmd) {
  t_cmd *new_cmd;

  new_cmd = init_struct();
  (*current_cmd)->next = new_cmd;
  *current_cmd = new_cmd;
}

void handle_redin_cmd(t_cmd *cmd, t_token *token) {
  if (token->next == NULL)
    return;
  cmd->infile = ft_strdup(token->next->content);
}

void handle_append_cmd(t_cmd *cmd, t_token *token) {
  if (token->next == NULL)
    return;
  cmd->outfile = ft_strdup(token->next->content);
  cmd->append = 1;
}

void handle_heredoc_cmd(t_cmd *cmd, t_token *token, int *i) {
  if (token->next == NULL)
    return;
  cmd->heredoc[*i] = ft_strdup(token->next->content);
  (*i)++;
}

void handle_redout_cmd(t_cmd *cmd, t_token *token) {
  if (token->next == NULL)
    return;
  cmd->outfile = ft_strdup(token->next->content);
  cmd->append = 0;
}

int open_outfile(char *filename, int append) {
  int result;

  result = 0;
  if (append == 0)
    result = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
  else
    result = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0777);
  if (result == -1)
    return (-1);
  return (result);
}

void fill_heredocs_from_tokens(t_token *token, t_cmd *cmd) {
  int i = 0;

  while (token && cmd) {
    if (token->type == HEREDOC && token->next) {
      cmd->heredoc[i] = ft_strdup(token->next->content);
      i++;
    } else if (token->type == PIPE) {
      cmd = cmd->next;
      i = 0;
    }
    token = token->next;
  }
}

t_cmd *parse_tokens(t_token *token) {
  t_cmd *head;
  t_cmd *current_cmd;

  if (!token)
    return (NULL);
  head = init_struct();
  if (!head)
    return (NULL);
  current_cmd = head;
  while (token != NULL) {
    if (token->type == WORD)
      handle_word_cmd(current_cmd, token);
    else if (token->type == PIPE) {
      handle_pipe_cmd(&current_cmd);
    } else if (token->type == REDIN) {
      if (!token->next)
        return (NULL);
      handle_redin_cmd(current_cmd, token);
      token = token->next;
    } else if (token->type == REDOUT) {
      if (!token->next)
        return (NULL);
      handle_redout_cmd(current_cmd, token);
      token = token->next;
    } else if (token->type == APPEND) {
      if (!token->next)
        return (NULL);
      handle_append_cmd(current_cmd, token);
      token = token->next;
    } else if (token->type == HEREDOC) {
      if (!token->next)
        return (NULL);
      token = token->next;
    }
    token = token->next;
  }
  return (head);
}

char **build_argv(t_cmd *cmd) {
  char **argv;
  int i;
  int j;
  int count;

  i = 0;
  j = 0;
  count = 1;
  if (!cmd) {
    argv = (char **)malloc(sizeof(char *));
    *argv = NULL;
    return argv;
  }
  while (cmd->args && cmd->args[i] != NULL) {
    count++;
    i++;
  }
  argv = (char **)malloc(sizeof(char *) * (count + 1));
  i = 0;
  argv[i++] = ft_strdup(cmd->cmd);
  while (cmd->args && cmd->args[j]) {
    argv[i++] = ft_strdup(cmd->args[j]);
    j++;
  }
  argv[i] = NULL;
  return (argv);
}
