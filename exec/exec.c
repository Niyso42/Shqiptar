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

void	execute_cmd(char *path, char **argv, t_data *data)
{
	if (execve(path, argv, data->env->env) == -1)
	{
		free_tab(argv);
		free(path);
		error_handling(3, data);
	}
}

char	*prepare_path(char *cmd, t_data *data)
{
	char	*path;
	char	**argv;

	if (!cmd || !cmd[0])
		error_handling(3, data);
	argv = ft_split(cmd, ' ');
	if (access(argv[0], X_OK) == 0)
		path = ft_strdup(argv[0]);
	else if (data->env)
		path = get_path(argv[0], data, argv);
	else
	{
		free_tab(argv);
		error_handling(3, data);
	}
	if (!path)
	{
		free_tab(argv);
		error_handling(3, data);
	}
	free_tab(argv);
	return (path);
}

int	*malloc_fds(int n)
{
	int	*fds;

	fds = malloc(sizeof(*fds) * 2 * (n - 1));
	if (fds == NULL)
		return (NULL);
	return (fds);
}

static int	count_commands(t_cmd *cmd)
{
	int		count;
	t_cmd	*tmp;

	count = 0;
	tmp = cmd;
	while (tmp->next)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

static int	create_pipes(int *fds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (pipe(&fds[i * 2]) == -1)
		{
			free(fds);
			return (-1);
		}
		i++;
	}
	return (0);
}

int	create_fds(t_cmd *cmd, int **fds_out)
{
	int	count;
	int	*fds;

	count = count_commands(cmd);
	fds = malloc(sizeof(int) * 2 * count);
	if (!fds)
		return (-1);
	if (create_pipes(fds, count) == -1)
		return (-1);
	*fds_out = fds;
	return (count);
}

void	redirect_input(t_cmd *cmd, int *fds, int index)
{
	int	fd;

	if (cmd->infile != NULL)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd == -1)
			return ;
		dup2(fd, 0);
		close(fd);
	}
	else if (cmd->heredoc)
	{
		dup2(cmd->heredoc_fd, 0);
		close(cmd->heredoc_fd);
		return ;
	}
	else if (index > 0)
	{
		dup2(fds[(index - 1) * 2], 0);
		close(fds[(index - 1) * 2]);
	}
}

void	redirect_output(t_cmd *cmd, int *fds, int index, int is_last)
{
	int	fd;

	if (cmd->outfile != NULL)
	{
		if (cmd->append == 0)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (fd == -1)
			return ;
		dup2(fd, 1);
		close(fd);
	}
	else if (!is_last)
	{
		dup2(fds[index * 2 + 1], 1);
		close(fds[index * 2 + 1]);
	}
}

static void	handle_builtin_execution(char **argv, t_data *data)
{
	if (is_parent_builtin(argv[0]))
	{
		free_tab(argv);
		free_data(data);
		exit(0);
	}
	if (is_builtin(argv[0]))
	{
		exec_builtin(argv, data);
		free_tab(argv);
		free_data(data);
		exit(0);
	}
}

static void	handle_command_execution(char **argv, t_data *data)
{
	char	*path;

	path = prepare_path(argv[0], data);
	if (!path)
	{
		ft_putstr_fd("Command not found\n", 2);
		free_tab(argv);
		free_data(data);
		exit(127);
	}
	execute_cmd(path, argv, data);
	free(path);
	free_tab(argv);
	free_data(data);
	exit(126);
}

void	execute_one_cmd(t_cmd *cmd, t_exec_context *ctx, t_data *data)
{
	char	**argv;

	redirect_input(cmd, ctx->fds, ctx->index);
	redirect_output(cmd, ctx->fds, ctx->index, ctx->is_last);
	argv = build_argv(cmd);
	if (!argv || !argv[0])
	{
		free_tab(argv);
		free_data(data);
		exit(0);
	}
	handle_builtin_execution(argv, data);
	handle_command_execution(argv, data);
}

static int	count_all_commands(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

static int	handle_single_builtin(t_cmd *cmd, t_data *data)
{
	char	**argv;

	if (cmd->cmd && is_parent_builtin(cmd->cmd))
	{
		argv = build_argv(cmd);
		exec_builtin(argv, data);
		free_tab(argv);
		return (1);
	}
	return (0);
}

static void	setup_signals_parent(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

static void	setup_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static void	restore_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

static void	close_pipe_fds(int *fds, int index, int count)
{
	if (index > 0)
		close(fds[(index - 1) * 2]);
	if (index < count - 1)
		close(fds[index * 2 + 1]);
}

static void	close_all_fds(int *fds, int count)
{
	int	i;

	i = 0;
	while (i < (count - 1) * 2)
		close(fds[i++]);
	if (fds)
		free(fds);
}

static void	handle_wait_status(int status, pid_t finished_pid, 
	pid_t last_pid, t_data *data)
{
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
}

static void	wait_for_children(int count, pid_t last_pid, t_data *data)
{
	int		i;
	int		status;
	pid_t	finished_pid;

	i = 0;
	while (i < count)
	{
		finished_pid = waitpid(-1, &status, 0);
		handle_wait_status(status, finished_pid, last_pid, data);
		i++;
	}
}

static void	cleanup_heredocs(t_cmd *head)
{
	t_cmd	*cleanup_cmd;

	cleanup_cmd = head;
	while (cleanup_cmd)
	{
		if (cleanup_cmd->heredoc_fd > 0)
		{
			close(cleanup_cmd->heredoc_fd);
			cleanup_cmd->heredoc_fd = 0;
		}
		cleanup_cmd = cleanup_cmd->next;
	}
}

static pid_t	fork_and_execute(t_cmd *cmd, t_exec_context *ctx, t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		setup_signals_child();
		execute_one_cmd(cmd, ctx, data);
		exit(0);
	}
	return (pid);
}

void	execute_all_cmd(t_cmd *cmd, t_data *data)
{
	int				count;
	pid_t			last_pid;
	int				*fds;
	t_cmd			*head;
	t_exec_context	ctx;
	int				index;

	head = cmd;
	count = count_all_commands(cmd);
	if (count == 1 && handle_single_builtin(cmd, data))
		return ;
	fds = NULL;
	if (count > 1 && create_fds(cmd, &fds) == -1)
		error_handling(6, data);
	setup_signals_parent();
	index = 0;
	last_pid = -1;
	while (cmd)
	{
		ctx = (t_exec_context){fds, index, (index == count - 1)};
		if (ctx.is_last)
			last_pid = fork_and_execute(cmd, &ctx, data);
		else
			fork_and_execute(cmd, &ctx, data);
		close_pipe_fds(fds, index, count);
		index++;
		cmd = cmd->next;
	}
	close_all_fds(fds, count);
	wait_for_children(count, last_pid, data);
	cleanup_heredocs(head);
	restore_signals();
}
