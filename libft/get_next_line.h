/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 16:55:52 by mubersan          #+#    #+#             */
/*   Updated: 2025/05/25 19:42:39 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZES
#  define BUFFER_SIZES  1
# endif

typedef struct t_struct
{
	char	buffer[BUFFER_SIZES];
	ssize_t	bytes_read;
	ssize_t	pos;
}	t_struct;

char	*get_next_line(int fd);
char	*ft_strjoin_gnl(char *line, char *buff);
size_t	ft_strlen_gnl(char *str);
char	*ft_substr_gnl(char *buff, size_t start);
char	*return_value(t_struct *b, char *line);
int		check_error(t_struct *b, char *line);
ssize_t	check_line(char *line);
ssize_t	check_pos(char *buffer, size_t pos);
char	*check_after_line(t_struct *b, char *line);
char	*read_line(int fd, t_struct *b, char *line);

#endif