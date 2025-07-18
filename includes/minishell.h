/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:29:38 by mubersan          #+#    #+#             */
/*   Updated: 2025/06/29 21:24:24 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <termios.h>
# include <sys/wait.h>
# include <unistd.h>

# define QUOTE 1
# define DQUOTE 2
# define PIPE 3
# define REDIN 4
# define REDOUT 5
# define WORD 6
# define APPEND 7
# define HEREDOC 8
# define BUFFER_SIZE 4096

extern int			g_exit_status;

typedef struct s_exit
{
	int				*exit;
}					t_exit;

typedef struct s_env
{
	char			**env;
}					t_env;

typedef struct s_token
{
	char			*content;
	int				type;
	struct s_token	*next;
}					t_token;

typedef struct s_exec_context
{
	int				*fds;
	int				index;
	int				is_last;
	int				total_cmds;
}					t_exec_context;

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;
	char			**heredoc;
	int				nb_heredoc;
	int				heredoc_fd;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_data
{
	t_exit			*exit;
	t_env			*env;
	t_token			*token;
	t_cmd			*cmd;
	int				should_exit;
}					t_data;

typedef struct s_pipeline_ctx
{
	t_data			*data;
	t_token			*tokens;
	int				*fds;
	int				count;
}					t_pipeline_ctx;

typedef struct s_wait_ctx
{
	int				status;
	pid_t			finished_pid;
	pid_t			last_pid;
	t_data			*data;
}					t_wait_ctx;

typedef struct s_expand_ctx
{
	char			*prompt;
	t_data			*data;
	char			*buffer;
	int				*i;
	int				*j;
}					t_expand_ctx;

typedef struct s_token_ctx
{
	char			*prompt;
	t_token			**head;
	char			*buffer;
	int				*i;
	int				*j;
	int				quote_state;
}					t_token_ctx;

typedef struct s_tokenize_vars
{
	t_token			**head;
	char			*buffer;
	int				i;
	int				j;
	int				quote_state;
}					t_tokenize_vars;

void				copy_env(t_data *data, char **envp);
void				execute_cmd(char *path, char **argv, t_data *data);
char				*prepare_path(char *cmd, t_data *data);
char				*get_path(char *cmd, t_data *data, char **argv);
void				free_env(t_env *env);
void				free_tab(char **str);
void				free_cmd(t_cmd *cmd);
void				free_tokens(t_token *token);
void 				free_data(t_data *data);
void				fill_heredocs_from_tokens(t_token *token, t_cmd *cmd);
void				allocate_heredoc(t_token *token, t_cmd *cmd);
void 				process_heredocs(t_cmd *cmd, t_data *data);
int 				handle_no_command(t_cmd *cmds, t_data *data, t_token *tokens, char *prompt);
int					error_handling(int err, t_data *data);
void 				handle_sigint_status(t_data *data);
void				ft_cd(char **args, t_data *data);
void				enable_echoctl(void);
void				disable_echoctl(void);
void				ft_env(t_data *data);
void				ft_pwd(t_data *data);
int					is_builtin(char *prompt);
int					is_n_option(const char *str);
void				ft_echo(char **args);
void				ft_unset(char **args, t_data *data);
void				ft_exit(char **args, t_data *data);
int					ft_export(t_data *data, char **args);
void				export_display(t_data *data);
void				ft_sort_ascii(char **array);
int					is_parent_builtin(char *prompt);
char				**ft_add_to_array(char **array, const char *new_str);
void				exec_builtin(char **argv, t_data *data);
void				free_array(char **array);
void				add_token(t_token **head, char *content, int type);
char				**ft_strdup_array(char **array);
int					is_valid_export_identifier(const char *s);
void				handle_dquote(char *prompt, t_token **head, int *j);
void				ft_puterror(const char *prefix, const char *arg,
						const char *msg);
void				handle_quote(char *prompt, t_token **head, int *j);
void				export_add_or_update(t_data *data, char *arg);
char 				*get_env_value(char *name, t_data *data);
char				*get_dollar_value(char *prompt, int j);
t_data				*init_data(char **envp);
void				handle_pipe(t_token **head, char buffer[BUFFER_SIZE],
						int *i, int *j);
void				handle_space(t_token **head, char buffer[BUFFER_SIZE],
						int *i, int *j);
void				handle_redin(t_token **head, char buffer[BUFFER_SIZE],
						int *i, int *j);
void				handle_redout(t_token **head, char buffer[BUFFER_SIZE],
						int *i, int *j);
void				handle_append(t_token **head, char buffer[BUFFER_SIZE],
						int *i, int *j);
void				handle_heredoc(t_token **head, char buffer[BUFFER_SIZE],
						int *i, int *j);
void				update_quote_state(char c, int *quote_state);
int 				create_heredoc_pipe(t_cmd *cmd, t_data *data);
t_cmd				*init_struct(void);
t_cmd				*parse_tokens(t_token *token);
t_token 			*tokenize(char *prompt, t_data *data);
int					validate_pipe_syntax(t_token *tokens);
char				**build_argv(t_cmd *cmd);
void				print_tokens(t_token *head);
void				print_cmds(t_cmd *cmd);
int					*malloc_fds(int n);
int					create_fds(t_cmd *cmd, int **fds_out);
void				redirect_input(t_cmd *cmd, int *fds, int index);
void				redirect_output(t_cmd *cmd, int *fds, int index,
						int is_last);
void				execute_one_cmd(t_cmd *cmd, t_exec_context *ctx,
						t_data *data, t_token *tokens);
void 				execute_all_cmd(t_cmd *cmd, t_data *data, t_token *tokens);
void				handle_sigint(int sig);

#endif