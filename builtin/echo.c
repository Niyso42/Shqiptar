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

int is_n_option(const char *str) {
  int i;

  if (!str || str[0] != '-' || str[1] != 'n')
    return (0);
  i = 2;
  while (str[i]) {
    if (str[i] != 'n')
      return (0);
    i++;
  }
  return (1);
}

void ft_echo(t_data *data, char **args) {
  int i;
  int newline;

  i = 1;
  newline = 1;
  while (args[i] && is_n_option(args[i])) {
    newline = 0;
    i++;
  }
  while (args[i]) {
    ft_putstr_fd(args[i], 1);
    if (args[i + 1])
      ft_putchar_fd(' ', 1);
    i++;
  }
  if (newline)
    ft_putchar_fd('\n', 1);
  *data->exit->exit = 0;
}
