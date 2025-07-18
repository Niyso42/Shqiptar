/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:47:19 by mubersan          #+#    #+#             */
/*   Updated: 2025/06/29 16:21:08 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_token(t_token **head, char *content, int type)
{
	t_token	*new_token;
	t_token	*tmp;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->content = ft_strdup(content);
	new_token->type = type;
	new_token->next = NULL;
	if (*head == NULL)
		*head = new_token;
	else
	{
		tmp = *head;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_token;
	}
}

static void	handle_dollar_expansion(t_expand_ctx *ctx)
{
	char	*varname;
	char	*value;
	int		k;

	(*ctx->j)++;
	varname = get_dollar_value(ctx->prompt, *ctx->j);
	value = get_env_value(varname, ctx->data);
	k = 0;
	if (value)
	{
		while (value[k] && *ctx->i < BUFFER_SIZE - 1)
			ctx->buffer[(*ctx->i)++] = value[k++];
	}
	*ctx->j += ft_strlen(varname);
	free(varname);
	free(value);
}

static int	handle_quotes(char *prompt, int *j, int *quote_state)
{
	if ((prompt[*j] == '\'' || prompt[*j] == '"') && *quote_state == 0)
	{
		update_quote_state(prompt[(*j)++], quote_state);
		return (1);
	}
	if ((prompt[*j] == '\'' && *quote_state == 1) 
		|| (prompt[*j] == '"' && *quote_state == 2))
	{
		update_quote_state(prompt[(*j)++], quote_state);
		return (1);
	}
	return (0);
}

static int	handle_redirections(t_token_ctx *ctx)
{
	if ((ctx->prompt[*ctx->j] == '<' && ctx->prompt[*ctx->j + 1] == '<') 
		&& ctx->quote_state == 0)
		return (handle_heredoc(ctx->head, ctx->buffer, ctx->i, ctx->j), 1);
	if (ctx->prompt[*ctx->j] == '<' && ctx->quote_state == 0)
		return (handle_redin(ctx->head, ctx->buffer, ctx->i, ctx->j), 1);
	if ((ctx->prompt[*ctx->j] == '>' && ctx->prompt[*ctx->j + 1] == '>') 
		&& ctx->quote_state == 0)
		return (handle_append(ctx->head, ctx->buffer, ctx->i, ctx->j), 1);
	if (ctx->prompt[*ctx->j] == '>' && ctx->quote_state == 0)
		return (handle_redout(ctx->head, ctx->buffer, ctx->i, ctx->j), 1);
	return (0);
}

static int	handle_special_chars(t_token_ctx *ctx)
{
	if (ctx->prompt[*ctx->j] == ' ' && ctx->quote_state == 0)
		return (handle_space(ctx->head, ctx->buffer, ctx->i, ctx->j), 1);
	if (ctx->prompt[*ctx->j] == '|' && ctx->quote_state == 0)
		return (handle_pipe(ctx->head, ctx->buffer, ctx->i, ctx->j), 1);
	return (handle_redirections(ctx));
}

void	print_tokens(t_token *head)
{
	t_token	*tmp;

	tmp = head;
	while (tmp)
	{
		printf("Content: %s | Type: %d\n", tmp->content, tmp->type);
		tmp = tmp->next;
	}
}

static void	init_tokenize_vars(t_tokenize_vars *vars, t_token **head)
{
	vars->head = head;
	vars->i = 0;
	vars->j = 0;
	vars->quote_state = 0;
	*head = NULL;
}

static int	process_token_char(char *prompt, t_data *data, t_tokenize_vars *vars)
{
	t_token_ctx	ctx;

	ctx.prompt = prompt;
	ctx.head = vars->head;
	ctx.buffer = vars->buffer;
	ctx.i = &vars->i;
	ctx.j = &vars->j;
	ctx.quote_state = vars->quote_state;
	if (handle_quotes(prompt, &vars->j, &vars->quote_state))
		return (1);
	if (handle_special_chars(&ctx))
		return (1);
	if (prompt[vars->j] == '$' && vars->quote_state != 1)
	{
		t_expand_ctx expand_ctx = {prompt, data, vars->buffer, &vars->i, &vars->j};
		handle_dollar_expansion(&expand_ctx);
	}
	else
		vars->buffer[vars->i++] = prompt[vars->j++];
	return (0);
}

static int	finalize_tokenize(t_tokenize_vars *vars)
{
	if (vars->quote_state != 0)
	{
		ft_putstr_fd("syntax error: unclosed quote\n", 2);
		return (0);
	}
	if (vars->i > 0)
	{
		vars->buffer[vars->i] = '\0';
		add_token(vars->head, vars->buffer, WORD);
	}
	return (1);
}

t_token	*tokenize(char *prompt, t_data *data)
{
	t_token			*head;
	char			buffer[BUFFER_SIZE];
	t_tokenize_vars	vars;

	init_tokenize_vars(&vars, &head);
	vars.buffer = buffer;
	while (prompt[vars.j])
	{
		if (process_token_char(prompt, data, &vars))
			continue ;
	}
	if (!finalize_tokenize(&vars))
	{
		free_tokens(head);
		return (NULL);
	}
	return (head);
}

int	validate_pipe_syntax(t_token *tokens)
{
	t_token	*current;
	int		consecutive_pipes;

	if (!tokens)
		return (1);
	if (tokens->type == PIPE)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `|'\n", 2);
		return (0);
	}
	current = tokens;
	consecutive_pipes = 0;
	while (current)
	{
		if (current->type == PIPE)
		{
			consecutive_pipes++;
			if (consecutive_pipes >= 2)
			{
				ft_putstr_fd("bash: syntax error near unexpected token `||'\n", 2);
				return (0);
			}
			if (!current->next)
			{
				ft_putstr_fd("bash: syntax error near unexpected token `|'\n", 2);
				return (0);
			}
		}
		else
			consecutive_pipes = 0;
		current = current->next;
	}
	return (1);
}
