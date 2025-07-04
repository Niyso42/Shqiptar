/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:40:06 by mubersan          #+#    #+#             */
/*   Updated: 2025/06/23 15:34:51 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int is_n_option(const char *str)
{
    if (!str || str[0] != '-' || str[1] != 'n')
        return 0;
    for (int i = 2; str[i]; i++)
    {
        if (str[i] != 'n')
            return 0;
    }
    return 1;
}

void ft_echo(char **args)
{
    int i = 1;
    int newline = 1;

    while (args[i] && is_n_option(args[i]))
    {
        newline = 0;
        i++;
    }
    while (args[i])
    {
        ft_putstr_fd(args[i], 1);
        if (args[i + 1])
            ft_putchar_fd(' ', 1);
        i++;
    }
    if (newline)
        ft_putchar_fd('\n', 1);
    g_exit_status = 0;
}
