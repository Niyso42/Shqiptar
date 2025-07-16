/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_prompt.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 19:20:50 by mubersan          #+#    #+#             */
/*   Updated: 2025/05/27 17:20:47 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_pipe(t_token **head, char buffer[BUFFER_SIZE], int *i, int *j)
{
	if (*i > 0)
	{
		buffer[*i] = '\0';
		add_token(head, buffer, WORD);
		*i = 0;
	}
	add_token(head, "|", PIPE);
	(*j)++;
}

void	handle_space(t_token **head, char buffer[BUFFER_SIZE], int *i, int *j)
{
	if (*i > 0)
	{
		buffer[*i] = '\0';
		add_token(head, buffer, WORD);
		*i = 0;
	}
	(*j)++;
}

void	handle_redin(t_token **head, char buffer[BUFFER_SIZE], int *i, int *j)
{
	if (*i > 0)
	{
		buffer[*i] = '\0';
		add_token(head, buffer, WORD);
		*i = 0;
	}
	add_token(head, "<", REDIN);
	(*j)++;
}

void	handle_redout(t_token **head, char buffer[BUFFER_SIZE], int *i, int *j)
{
	if (*i > 0)
	{
		buffer[*i] = '\0';
		add_token(head, buffer, WORD);
		*i = 0;
	}
	add_token(head, ">", REDOUT);
	(*j)++;
}

void	handle_append(t_token **head, char buffer[BUFFER_SIZE], int *i, int *j)
{
	if (*i > 0)
	{
		buffer[*i] = '\0';
		add_token(head, buffer, WORD);
		*i = 0;
	}
	add_token(head, ">>", APPEND);
	(*j) += 2;
}

void	handle_heredoc(t_token **head, char buffer[BUFFER_SIZE], int *i, int *j)
{
	if (*i > 0)
	{
		buffer[*i] = '\0';
		add_token(head, buffer, WORD);
		*i = 0;
	}
	add_token(head, "<<", HEREDOC);
	(*j) += 2;
}

void	update_quote_state(char c, int *quote_state)
{
	if (c == '\'' && *quote_state == 0)
		*quote_state = 1;
	else if (c == '\'' && *quote_state == 1)
		*quote_state = 0;
	else if (c == '"' && *quote_state == 0)
		*quote_state = 2;
	else if (c == '"' && *quote_state == 2)
		*quote_state = 0;
}
