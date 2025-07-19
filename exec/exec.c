/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:30:39 by mubersan          #+#    #+#             */
/*   Updated: 2025/06/29 21:21:52 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void cleanup_and_exit(char **argv, t_data *data, int exit_code) {
  free_tab(argv);
  free_cmd(data->cmd);
  free_tokens(data->token);
  *data->exit->exit = exit_code;
  free_data(data);
  exit(exit_code);
}

static void handle_directory_error(char **argv, t_data *data) {
  ft_putstr_fd("minishell: ", 2);
  ft_putstr_fd(argv[0], 2);
  ft_putstr_fd(": Is a directory\n", 2);
  cleanup_and_exit(argv, data, 126);
}

static void handle_command_not_found(char **argv, t_data *data) {
  ft_putstr_fd(argv[0], 2);
  ft_putstr_fd(": ", 2);
  ft_putstr_fd("command not found\n", 2);
  cleanup_and_exit(argv, data, 127);
}

static void close_unused_fds(t_exec_context *ctx) {
  int i;

  if (ctx->fds) {
    i = 0;
    while (i < (ctx->total_cmds - 1) * 2)
      close(ctx->fds[i++]);
    free(ctx->fds);
  }
}

static void handle_empty_argv(char **argv, t_data *data) {
  if (!argv || !argv[0])
    cleanup_and_exit(argv, data, 0);
}

static void handle_execve_error(char **argv) {
  if (errno == EACCES) {
    ft_putstr_fd(argv[0], 2);
    ft_putstr_fd(": Permission denied\n", 2);
  } else if (errno == ENOEXEC) {
    ft_putstr_fd(argv[0], 2);
    ft_putstr_fd(": cannot execute binary file: Exec format error\n", 2);
  } else if (errno == ENOENT) {
    ft_putstr_fd(argv[0], 2);
    ft_putstr_fd(": No such file or directory\n", 2);
  } else if (errno == EISDIR) {
    ft_putstr_fd(argv[0], 2);
    ft_putstr_fd(": Is a directory\n", 2);
  } else
    perror("execve failed");
}

void execute_cmd(char *path, char **argv, t_data *data) {
  if (execve(path, argv, data->env->env) == -1) {
    handle_execve_error(argv);
    free(path);
    cleanup_and_exit(argv, data, 126);
  }
}

static int is_directory(char *path) {
  struct stat stat_buf;

  if (stat(path, &stat_buf) == 0 && S_ISDIR(stat_buf.st_mode))
    return (1);
  return (0);
}

char *prepare_path(char *cmd, t_data *data) {
  char *path;
  char **argv;

  if (!cmd || !cmd[0])
    return (NULL);
  argv = ft_split(cmd, ' ');
  if (access(argv[0], F_OK) == 0) {
    if (is_directory(argv[0])) {
      free_tab(argv);
      return (NULL);
    }
    path = ft_strdup(argv[0]);
  } else if (data->env)
    path = get_path(argv[0], data, argv);
  else {
    free_tab(argv);
    return (NULL);
  }
  if (!path) {
    free_tab(argv);
    return (NULL);
  }
  free_tab(argv);
  return (path);
}

int *malloc_fds(int n) {
  int *fds;

  fds = malloc(sizeof(*fds) * 2 * (n - 1));
  if (fds == NULL)
    return (NULL);
  return (fds);
}

static int count_all_commands(t_cmd *cmd) {
  int count;

  count = 0;
  while (cmd) {
    count++;
    cmd = cmd->next;
  }
  return (count);
}

static int create_pipes(int *fds, int count) {
  int i;

  i = 0;
  while (i < count) {
    if (pipe(&fds[i * 2]) == -1) {
      free(fds);
      return (-1);
    }
    i++;
  }
  return (0);
}

int create_fds(t_cmd *cmd, int **fds_out) {
  int count;
  int *fds;

  count = count_all_commands(cmd);
  if (count <= 1) {
    *fds_out = NULL;
    return (count);
  }
  fds = malloc(sizeof(int) * 2 * (count - 1));
  if (!fds)
    return (-1);
  if (create_pipes(fds, count - 1) == -1)
    return (-1);
  *fds_out = fds;
  return (count);
}

void redirect_input(t_cmd *cmd, int *fds, int index) {
  int fd;

  if (cmd->infile != NULL) {
    fd = open(cmd->infile, O_RDONLY);
    if (fd == -1)
      return;
    dup2(fd, 0);
    close(fd);
  } else if (cmd->heredoc) {
    dup2(cmd->heredoc_fd, 0);
    close(cmd->heredoc_fd);
    return;
  } else if (index > 0) {
    dup2(fds[(index - 1) * 2], 0);
    close(fds[(index - 1) * 2]);
  }
}

void redirect_output(t_cmd *cmd, int *fds, int index, int is_last) {
  int fd;

  if (cmd->outfile != NULL) {
    if (cmd->append == 0)
      fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    else
      fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0777);
    if (fd == -1)
      return;
    dup2(fd, 1);
    close(fd);
  } else if (!is_last) {
    dup2(fds[index * 2 + 1], 1);
    close(fds[index * 2 + 1]);
  }
}

static void handle_builtin_execution(char **argv, t_data *data) {
  // if (is_parent_builtin(argv[0])) {
  //   cleanup_and_exit(argv, data, 0);
  // }
  if (is_builtin(argv[0]) || is_parent_builtin(argv[0])) {
    exec_builtin(argv, data);
    cleanup_and_exit(argv, data, *data->exit->exit);
  }
}

static int is_directory_path(char *cmd) {
  if (!cmd)
    return (0);
  if (ft_strcmp(cmd, "/") == 0)
    return (1);
  if (ft_strcmp(cmd, "./") == 0)
    return (1);
  if (ft_strcmp(cmd, "../") == 0)
    return (1);
  return (0);
}

static void handle_command_execution(char **argv, t_data *data) {
  char *path;

  if (is_directory_path(argv[0]))
    handle_directory_error(argv, data);
  path = prepare_path(argv[0], data);
  if (!path)
    handle_command_not_found(argv, data);
  execute_cmd(path, argv, data);
  cleanup_and_exit(argv, data, 126);
}

void execute_one_cmd(t_cmd *cmd, t_exec_context *ctx, t_data *data,
                     t_token *tokens) {
  char **argv;

  data->token = tokens;
  data->cmd = cmd;
  redirect_input(cmd, ctx->fds, ctx->index);
  redirect_output(cmd, ctx->fds, ctx->index, ctx->is_last);
  close_unused_fds(ctx);
  argv = build_argv(cmd);
  handle_empty_argv(argv, data);
  handle_builtin_execution(argv, data);
  handle_command_execution(argv, data);
  cleanup_and_exit(argv, data, 0);
}

// void execute_single_cmd(t_cmd *cmd, t_exec_context *ctx, t_data *data,
//                      t_token *tokens) {
//   char **argv;

//   data->token = tokens;
//   data->cmd = cmd;
//   redirect_input(cmd, ctx->fds, ctx->index);
//   redirect_output(cmd, ctx->fds, ctx->index, ctx->is_last);
//   close_unused_fds(ctx);
//   argv = build_argv(cmd);
//   if (!1)
//     return;
//   if (is_builtin(argv[0]) || is_parent_builtin(argv[0]))
//     return (exec_builtin(argv, data));
//   // handle_builtin_execution(argv, data) {
//   handle_command_execution(argv, data);
//   cleanup_and_exit(argv, data, 0);
// }

// if (is_builtin(argv[0]) || is_parent_builtin(argv[0]))

// static int handle_single_builtin(t_cmd *cmd, t_data *data) {
//   char **argv;

//   if (cmd->cmd && is_parent_builtin(cmd->cmd)) {
//     argv = build_argv(cmd);
//     exec_builtin(argv, data);
//     free_tab(argv);
//     return (1);
//   }
//   return (0);
// }

static void setup_signals_parent(void) {
  signal(SIGINT, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
}

static void setup_signals_child(void) {
  signal(SIGINT, SIG_DFL);
  signal(SIGQUIT, SIG_DFL);
}

static void restore_signals(void) {
  signal(SIGINT, handle_sigint);
  signal(SIGQUIT, SIG_IGN);
}

static void close_pipe_fds(int *fds, int index, int count) {
  if (index > 0)
    close(fds[(index - 1) * 2]);
  if (index < count - 1)
    close(fds[index * 2 + 1]);
}

static void close_all_fds(int *fds, int count) {
  int i;

  i = 0;
  while (i < (count - 1) * 2)
    close(fds[i++]);
  if (fds)
    free(fds);
}

static void handle_wait_status(t_wait_ctx *ctx) {
  if (ctx->finished_pid == ctx->last_pid) {
    if (WIFEXITED(ctx->status))
      *ctx->data->exit->exit = WEXITSTATUS(ctx->status);
    else if (WIFSIGNALED(ctx->status))
      *ctx->data->exit->exit = 128 + WTERMSIG(ctx->status);
    if (WIFSIGNALED(ctx->status) && WTERMSIG(ctx->status) == SIGINT)
      write(STDOUT_FILENO, "\n", 1);
    if (WIFSIGNALED(ctx->status) && WTERMSIG(ctx->status) == SIGQUIT)
      write(STDOUT_FILENO, "Quit\n", 5);
  }
}

static void wait_for_children(int count, pid_t last_pid, t_data *data) {
  int i;
  int status;
  pid_t finished_pid;
  t_wait_ctx ctx;

  i = 0;
  while (i < count) {
    finished_pid = waitpid(-1, &status, 0);
    ctx = (t_wait_ctx){status, finished_pid, last_pid, data};
    handle_wait_status(&ctx);
    i++;
  }
}

static void cleanup_heredocs(t_cmd *head) {
  t_cmd *cleanup_cmd;

  cleanup_cmd = head;
  while (cleanup_cmd) {
    if (cleanup_cmd->heredoc_fd > 0) {
      close(cleanup_cmd->heredoc_fd);
      cleanup_cmd->heredoc_fd = 0;
    }
    cleanup_cmd = cleanup_cmd->next;
  }
}

static pid_t fork_and_execute(t_cmd *cmd, t_exec_context *ctx, t_data *data,
                              t_token *tokens) {
  pid_t pid;

  pid = fork();
  if (pid == 0) {
    setup_signals_child();
    execute_one_cmd(cmd, ctx, data, tokens);
    exit(0);
  }
  return (pid);
}

// static void execute_no_fork(t_cmd *cmd, t_exec_context *ctx, t_data *data,
//                               t_token *tokens) {
//   execute_one_cmd(cmd, ctx, data, tokens);
// }

static int setup_execution_context(t_cmd *cmd, t_data *data, int **fds) {
  int count;

  count = count_all_commands(cmd);
  *fds = NULL;
  if (count > 0 && create_fds(cmd, fds) == -1)
    error_handling(6, data);
  setup_signals_parent();
  return (count);
}

// static void execute_single(t_cmd *cmd, t_pipeline_ctx *ctx) {
//   t_exec_context exec_ctx;

//   exec_ctx = (t_exec_context){ctx->fds, 0, 1 == 1, 1};
//   execute_one_cmd(cmd, &exec_ctx, ctx->data, ctx->tokens);
// }

static void execute_single_builtin(t_cmd *cmd, t_exec_context *ctx,
                                   t_data *data, char **argv) {
  int stdin;
  int stdout;

  stdin = dup(STDIN_FILENO);
  stdout = dup(STDOUT_FILENO);
  redirect_input(cmd, NULL, ctx->index);
  redirect_output(cmd, NULL, ctx->index, 1 == 1);
  if (argv && argv[0]) {
    exec_builtin(argv, data);
  }
  dup2(stdin, STDIN_FILENO);
  dup2(stdout, STDOUT_FILENO);
  close(stdin);
  close(stdout);
}

static void execute_pipeline(t_cmd *cmd, t_pipeline_ctx *ctx) {
  pid_t last_pid;
  t_exec_context exec_ctx;
  int index;
  t_cmd *current;
  char **argv;

  index = 0;
  last_pid = -1;
  current = cmd;
  if (ctx->count == 1) {
    argv = build_argv(cmd);
    if (!argv || !argv[0] || is_builtin(argv[0]) ||
        is_parent_builtin(argv[0])) {
      exec_ctx = (t_exec_context){ctx->fds, index, (index == ctx->count - 1),
                                  ctx->count};
      execute_single_builtin(cmd, &exec_ctx, ctx->data, argv);
      free_array(argv);
      return;
    }
    free_array(argv);
  }
  while (current) {
    exec_ctx = (t_exec_context){ctx->fds, index, (index == ctx->count - 1),
                                ctx->count};
    if (exec_ctx.is_last)
      last_pid = fork_and_execute(current, &exec_ctx, ctx->data, ctx->tokens);
    else
      fork_and_execute(current, &exec_ctx, ctx->data, ctx->tokens);
    close_pipe_fds(ctx->fds, index, ctx->count);
    index++;
    current = current->next;
  }
  close_all_fds(ctx->fds, ctx->count);
  wait_for_children(ctx->count, last_pid, ctx->data);
}

void execute_all_cmd(t_cmd *cmd, t_data *data, t_token *tokens) {
  int count;
  int *fds;
  t_cmd *head;
  t_pipeline_ctx ctx;

  head = cmd;
  count = setup_execution_context(cmd, data, &fds);
  ctx = (t_pipeline_ctx){data, tokens, fds, count};
  execute_pipeline(cmd, &ctx);
  cleanup_heredocs(head);
  restore_signals();
}
