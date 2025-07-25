/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:51:07 by mubersan          #+#    #+#             */
/*   Updated: 2025/06/29 22:00:25 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			g_exit_status = 0;

static void	process_commands(t_data *data, char *prompt)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = tokenize(prompt, data);
	if (!tokens)
		return ;
	cmds = parse_tokens(tokens);
	if (!cmds)
	{
		free_tokens(tokens);
		return ;
	}
	allocate_heredoc(tokens, cmds);
	fill_heredocs_from_tokens(tokens, cmds);
	process_heredocs(cmds, data);
	// if (handle_no_command(cmds, data, tokens, prompt)) ;
		// return ;
	execute_all_cmd(cmds, data, tokens);
	free_tokens(tokens);
	free_cmd(cmds);
}

static int	handle_prompt_input(t_data *data, char *prompt)
{
	if (!prompt)
	{
		ft_putstr_fd("exit\n", 1);
		free_data(data);
		return (1);
	}
	if (*prompt)
		add_history(prompt);
	process_commands(data, prompt);
	free(prompt);
	return (0);
}

static int	initialize_minishell(char **envp, t_data **data)
{
	if (!envp)
		return (1);
	*data = init_data(envp);
	if (!*data)
		return (1);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}

static int	run_minishell_loop(t_data *data)
{
	char	*prompt;

	while ((prompt = readline("minishell$ ")))
	{
		handle_sigint_status(data);
		if (handle_prompt_input(data, prompt))
			return (0);
		if (data->should_exit)
			break ;
	}
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	t_data	*data;
	int		exit_code;

	(void)av;
	if (ac != 1)
		return (1);
	if (initialize_minishell(envp, &data))
		return (1);
	run_minishell_loop(data);
	rl_clear_history();
	exit_code = *data->exit->exit;
	free_data(data);
	return (exit_code);
}
