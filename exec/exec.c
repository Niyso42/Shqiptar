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

void execute_cmd(char *path, char **argv, t_data *data)
{
    if (execve(path, argv, data->env->env) == -1)
    {
        free_tab(argv);
        free(path);
        error_handling(3);
    }
}

char *prepare_path(char *cmd, t_data *data)
{
    char *path;
    char **argv;

    if (!cmd || !cmd[0])
        error_handling(3);
    argv = ft_split(cmd, ' ');
    if (access(argv[0], X_OK) == 0)
        path = ft_strdup(argv[0]);
    else if (data->env)
        path = get_path(argv[0], data, argv);
    else
    {
        free_tab(argv);
        error_handling(3);
    }
    if (!path)
    {
        free_tab(argv);
        error_handling(3);
    }
    free_tab(argv);
    return (path);
}

int *malloc_fds(int n)
{
	int *fds;

	fds = malloc(sizeof(*fds) * 2 * (n - 1));
	if (fds == NULL)
		return (NULL);
	return (fds);
}

int create_fds(t_cmd *cmd, int **fds_out)
{
	int count = 0;
	int i = 0;
	int *fds;

	t_cmd *tmp = cmd;
	while (tmp->next)
	{
		count++;
		tmp = tmp->next;
	}
	fds = malloc(sizeof(int) * 2 * count);
	if (!fds)
		return (-1);
	while (i < count)
	{
		if (pipe(&fds[i * 2]) == -1)
		{
			free(fds);
			return (-1);
		}
		i++;
	}
	*fds_out = fds;
	return (count);
}

void redirect_input(t_cmd *cmd, int *fds, int index)
{
	int fd;
	if (cmd->infile != NULL)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd == -1)
			error_handling(3);
		dup2(fd, 0);
		close(fd);
	}
	else if (cmd->heredoc)
	{
		dup2(cmd->heredoc_fd, 0);
		close(cmd->heredoc_fd);
		return;
	}
	else if (index > 0)
	{
		dup2(fds[(index - 1) * 2], 0);
		close(fds[(index - 1) * 2]);
	}
}

void redirect_output(t_cmd *cmd, int *fds, int index, int is_last)
{
	int fd;
	if (cmd->outfile != NULL)
	{
		if (cmd->append == 0)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (fd == -1)
			error_handling(3);
		dup2(fd, 1);
		close(fd);
	}
	else if (!is_last)
	{
		dup2(fds[index * 2 + 1], 1);
		close(fds[index * 2 + 1]);
	}
}

void execute_one_cmd(t_cmd *cmd, t_exec_context *ctx, t_data *data)
{
    char **argv;
    char *path;

    redirect_input(cmd, ctx->fds, ctx->index);
    redirect_output(cmd, ctx->fds, ctx->index, ctx->is_last);

    argv = build_argv(cmd);

    if (is_parent_builtin(argv[0]))
    {
        free_tab(argv);
        exit(0);
    }
    if (is_builtin(argv[0]))
    {
        exec_builtin(argv, data);
        free_tab(argv);
        exit(0);
    }

    path = prepare_path(argv[0], data);
    if (!path)
    {
        free_tab(argv);
        error_handling(3);
    }
    execute_cmd(path, argv, data);
}

void execute_all_cmd(t_cmd *cmd, t_data *data)
{
    int count = 0;
    pid_t pid;
    int *fds;
    int index = 0;
    int status;
    int is_last;
    int i = 0;
    pid_t last_pid = -1;
    pid_t finished_pid;
    t_cmd *head = cmd;
    char **argv;
    t_exec_context ctx;

    while (cmd)
    {
        count++;
        cmd = cmd->next;
    }
    cmd = head;

    if (count == 1 && cmd->cmd && is_parent_builtin(cmd->cmd))
    {
        argv = build_argv(cmd);
        exec_builtin(argv, data);
        free_tab(argv);
        return;
    }

    create_fds(cmd, &fds);

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

    while (cmd)
    {
        is_last = (index == count - 1);

        ctx.fds = fds;
        ctx.index = index;
        ctx.is_last = is_last;

        pid = fork();
        if (pid == 0)
        {
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            execute_one_cmd(cmd, &ctx, data);
            exit(0);
        }
        if (is_last)
            last_pid = pid;

        if (index > 0)
            close(fds[(index - 1) * 2]);
        if (index < count - 1)
            close(fds[index * 2 + 1]);

        index++;
        cmd = cmd->next;
    }

    while (i < (count - 1) * 2)
        close(fds[i++]);
    free(fds);

    i = 0;
    while (i < count)
    {
        finished_pid = waitpid(-1, &status, 0);
        if (finished_pid == last_pid)
        {
			if (WIFEXITED(status))
				*data->exit->exit = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				*data->exit->exit = 128 + WTERMSIG(status);
            if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
                write(STDOUT_FILENO, "\n", 1);
            if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
                write(STDOUT_FILENO, "Quit\n", 5);
        }
        i++;
    }
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);
}
