/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 20:31:59 by mubersan          #+#    #+#             */
/*   Updated: 2025/06/29 22:00:04 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void allocate_heredoc(t_token *token, t_cmd *cmd)
{
	t_cmd *current = cmd;

	while (token && current)
	{
		if (token->type == HEREDOC)
		{
			current->nb_heredoc++;
		}
		else if (token->type == PIPE)
		{
			current = current->next;
		}
		token = token->next;
	}
	current = cmd;
	while (current)
	{
		if (current->nb_heredoc > 0)
			current->heredoc = malloc(sizeof(char *) * (current->nb_heredoc));
		current = current->next;
	}
}

void process_heredocs(t_cmd *cmd, t_data *data)
{
    t_cmd *tmp = cmd;
    while (tmp)
    {
        if (tmp->nb_heredoc > 0) 
        {
            if (create_heredoc_pipe(tmp, data) == -1)
                error_handling(3, data);
        }
        tmp = tmp->next;
    }
}

int handle_no_command(t_cmd *cmds, t_data *data, t_token *tokens, char *prompt)
{
    t_cmd *check = cmds;

    (void)prompt;
    while (check)
    {
        if (check->cmd != NULL)
            return 0; 
        check = check->next;
    }
    free_cmd(cmds);
    *data->exit->exit = 0; 
    free_tokens(tokens);
    return 1;
}

void handle_heredoc_eof(char *delimiter)
{
    ft_putchar_fd('\n', 1);
    ft_putstr_fd("bash: warning: here-document delimited by end-of-file (wanted `", 2);
    ft_putstr_fd(delimiter, 2);
    ft_putstr_fd("')\n", 2);
}

int process_heredoc_line(char *line, char *delimiter, int fd, int is_last)
{
    if (line[ft_strlen(line) - 1] == '\n')
        line[ft_strlen(line) - 1] = '\0';
    
    if (ft_strcmp(line, delimiter) == 0)
    {
        free(line);
        return (1);
    }
    
    if (is_last)
    {
        ft_putstr_fd(line, fd);
        ft_putstr_fd("\n", fd);
    }
    free(line);
    return (0);
}

void heredoc_child_process(t_cmd *cmd, int write_fd)
{
    char *line;
    int i = 0;

    disable_echoctl();
    signal(SIGINT, SIG_DFL);
    
    while (i < cmd->nb_heredoc)
    {
        while (1)
        {
            ft_putstr_fd("> ", 1);
            line = get_next_line(0);
            if (!line)
            {
                handle_heredoc_eof(cmd->heredoc[i]);
                break;
            }
            if (process_heredoc_line(line, cmd->heredoc[i], write_fd, 
                                   (i == cmd->nb_heredoc - 1)))
                break;
        }
        i++;
    }
    close(write_fd);
    exit(0);
}

int handle_heredoc_parent(pid_t pid, int *fds, t_data *data)
{
    int status;

    close(fds[1]);
    waitpid(pid, &status, 0);
    enable_echoctl();
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);
    
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
        *data->exit->exit = 130;
        return (1);
    }
    return (0);
}

int create_heredoc_pipe(t_cmd *cmd, t_data *data)
{
    int fds[2];
    pid_t pid;

    if (pipe(fds) == -1)
        return (-1);
    
    pid = fork();
    if (pid == -1)
        return (-1);

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    
    if (pid == 0)
    {
        close(fds[0]);
        heredoc_child_process(cmd, fds[1]);
    }
    
    if (handle_heredoc_parent(pid, fds, data) == 1)
        return (1);
    
    cmd->heredoc_fd = fds[0];
    return (0);
}
