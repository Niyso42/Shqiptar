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

// int	main(int ac, char **av, char **envp)
// {
// 	char	*prompt;
// 	pid_t	id;
// 	t_env	env;

// 	(void)av;
// 	env.env = NULL;
// 	copy_env(&env, envp);
// 	if (ac != 1)
// 		return (free_env(&env), 1);
// 	while ((prompt = readline("minishell$ ")))
// 	{
// 		if (!prompt)
// 			return (ft_putstr_fd("exit\n", 1), free_env(&env), 0);
// 		if (is_builtin(prompt))
// 			exec_builtin(prompt, &env);
// 		else if ((id = fork()) == 0)
// 		{
// 			prepare_and_execute(prompt, &env);
// 			exit(0);
// 		}
// 		else if (id > 0)
// 			waitpid(id, NULL, 0);
// 		if (*prompt)
// 			add_history(prompt);
// 		free(prompt);
// 	}
// 	return (free_env(&env), 0);
// }

int main(int ac, char **av, char **envp)
{
    char    *prompt;
    t_data  *data;
    t_token *tokens;
    t_cmd   *cmds;

    (void)av;
    if (ac != 1)
        return (1);

    data = init_data(envp);
    if (!data)
        return (1);

    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, SIG_IGN);
    while ((prompt = readline("minishell$ ")))
    {
        handle_sigint_status(data);
        if (!prompt)
            return (ft_putstr_fd("exit\n", 1), free_data(data), 0);
        if (*prompt)
            add_history(prompt);

        tokens = tokenize(prompt, data);
        if (!tokens)
        {
            free(prompt);
            continue;
        }

        cmds = parse_tokens(tokens);
        if (!cmds)
        {
            free_tokens(tokens);
            free(prompt);
            continue;
        }

        allocate_heredoc(tokens, cmds);
        fill_heredocs_from_tokens(tokens, cmds);
        process_heredocs(cmds, data);
        
        if (handle_no_command(cmds, data, tokens, prompt))
            continue;

        execute_all_cmd(cmds, data);
        free_tokens(tokens);
        free_cmd(cmds);
        free(prompt);
        
        // Vérifier si le shell doit se terminer
        if (data->should_exit)
            break;
    }
    rl_clear_history();
    int exit_code = *data->exit->exit;
    free_data(data);
    return (exit_code);
}





/************************************************POUR TEST LES TOKENS ********************************************/


// int main(int ac, char **av, char **envp)
// {
//  	char *prompt = "cat << EOF << EOF1";
// 	t_cmd *cmds;
// 	t_token *tokens;
// 	t_env	env;
// 	env.env = NULL;
//  	copy_env(&env, envp);
// 	tokens = tokenize(prompt, &env); 
// 	cmds = parse_tokens(tokens);
// 	(void)av;
// 	(void)ac;
// 	copy_env(&env, envp);
//     execute_all_cmd(cmds, &env);
// }

/************************************************POUR TEST LES CMD ********************************************/

// int main(void)
// {
//     // t_token t12 = {"file2", WORD, NULL};
//     // t_token t11 = {">>", APPEND, &t12};
//     // t_token t10 = {"-l", WORD, &t11};
//     // t_token t9 = {"wc", WORD, &t10};
//     // t_token t8 = {"|", PIPE, &t9};
//     // t_token t7 = {"hello world", WORD, &t8};
//     // t_token t6 = {"grep", WORD, &t7};
//     t_token t5 = {"EOF1", WORD, NULL};
//     t_token t4 = {"<<", HEREDOC, &t5};
//     t_token t3 = {"EOF", WORD, &t4};
//     t_token t2 = {"<<", HEREDOC, &t3};
//     t_token t1 = {"cat", WORD, &t2};

//     t_cmd *cmds = parse_tokens(&t1);
//     print_cmds(cmds);

//     return 0;
// }

/************************************************SIMULATION DE CMD*********************************************/


// int main(void)
// {
//     t_cmd *cmd;
//     char **argv;
//     int i = 0;

//     cmd = init_struct();
//     cmd->cmd = strdup("grep");

//     cmd->args = malloc(sizeof(char *) * 3); 
//     cmd->args[0] = strdup("-v");
//     cmd->args[1] = strdup("SHLVL");
//     cmd->args[2] = NULL;

//     argv = build_argv(cmd);

//     while (argv[i])
//     {
//         printf("argv[%d] = %s\n", i, argv[i]);
//         i++;
//     }

//     return 0;
// }