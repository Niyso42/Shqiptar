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

t_token *tokenize(char *prompt, t_data *data)
{
    t_token *head;
    char buffer[BUFFER_SIZE];
    int i, j, k;
    int quote_state;
    char *varname;
    char *value;

    head = NULL;
    i = 0;
    j = 0;
    quote_state = 0;
    while (prompt[j])
    {
        if ((prompt[j] == '\'' || prompt[j] == '"') && quote_state == 0)
            update_quote_state(prompt[j++], &quote_state);
        else if ((prompt[j] == '\'' && quote_state == 1) || (prompt[j] == '"' && quote_state == 2))
            update_quote_state(prompt[j++], &quote_state);
        else if (prompt[j] == ' ' && quote_state == 0)
            handle_space(&head, buffer, &i, &j);
        else if (prompt[j] == '|' && quote_state == 0)
            handle_pipe(&head, buffer, &i, &j);
        else if ((prompt[j] == '<' && prompt[j + 1] == '<') && quote_state == 0)
            handle_heredoc(&head, buffer, &i, &j);
        else if (prompt[j] == '<' && quote_state == 0)
            handle_redin(&head, buffer, &i, &j);
        else if ((prompt[j] == '>' && prompt[j + 1] == '>') && quote_state == 0)
            handle_append(&head, buffer, &i, &j);
        else if (prompt[j] == '>' && quote_state == 0)
            handle_redout(&head, buffer, &i, &j);
        else if (prompt[j] == '$' && quote_state != 1)
        {
            j++;
            varname = get_dollar_value(prompt, j);
            value = get_env_value(varname, data);
            k = 0;
            if (value)
                while (value[k] && i < BUFFER_SIZE - 1)
                    buffer[i++] = value[k++];
            j += ft_strlen(varname);
            free(varname);
            free(value);
        }
        else
            buffer[i++] = prompt[j++];
    }
    if (quote_state != 0)
    {
        ft_putstr_fd("syntax error: unclosed quote\n", 2);
        return (NULL);
    }
    if (i > 0)
    {
        buffer[i] = '\0';
        add_token(&head, buffer, WORD);
    }
    return (head);
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
