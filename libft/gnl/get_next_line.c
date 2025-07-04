/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 16:03:53 by mubersan          #+#    #+#             */
/*   Updated: 2025/05/25 19:43:03 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../get_next_line.h"

ssize_t	check_line(char *line)
{
	size_t	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

ssize_t	check_pos(char *buffer, size_t pos)
{
	while (buffer[pos])
	{
		if (buffer[pos] == '\n')
			return (pos);
		pos++;
	}
	return (0);
}

char	*check_after_line(t_struct *b, char *line)
{
	free(line);
	line = ft_substr_gnl(b->buffer, b->pos);
	if (!line)
		return (NULL);
	return (line);
}

char	*read_line(int fd, t_struct *b, char *line)
{
	b->bytes_read = read(fd, b->buffer, BUFFER_SIZES);
	if (b->bytes_read < 0)
	{
		free(line);
		return (NULL);
	}
	if (b->bytes_read == 0)
	{
		if (*line != '\0')
			return (line);
		free(line);
		return (NULL);
	}
	b->buffer[b->bytes_read] = '\0';
	line = ft_strjoin_gnl(b->buffer, line);
	if (!line)
		return (NULL);
	return (line);
}

char	*get_next_line(int fd)
{
	static t_struct	b[1024];
	char			*line;

	if (fd < 0 || fd > 1023 || BUFFER_SIZES <= 0)
		return (NULL);
	line = malloc(1);
	if (!line)
		return (NULL);
	line[0] = '\0';
	if (b[fd].pos < b[fd].bytes_read)
	{
		line = check_after_line(&b[fd], line);
		if (!line)
			return (NULL);
		if (check_line(line) == 1)
			return (b[fd].pos = check_pos(b[fd].buffer, b[fd].pos) + 1, line);
	}
	while (check_line(line) == 0)
	{
		line = read_line(fd, &b[fd], line);
		if (check_error(&b[fd], line) > 0)
			return (return_value(&b[fd], line));
	}
	return (line);
}
