/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 16:15:50 by mubersan          #+#    #+#             */
/*   Updated: 2025/05/25 19:42:48 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../get_next_line.h"

char	*ft_strjoin_gnl(char *buff, char *line)
{
	char	*str;
	size_t	i;
	size_t	j;
	size_t	z;

	z = 0;
	while (buff[z] && buff[z] != '\n')
		z++;
	if (buff[z] == '\n')
		z++;
	i = -1;
	j = 0;
	str = malloc(z + ft_strlen_gnl(line) + 1);
	if (!str)
		return (free(line), NULL);
	while (line[++i])
		str[i] = line[i];
	while (buff[j] != '\n' && buff[j])
		str[i++] = buff[j++];
	if (buff[j] == '\n')
		str[i++] = '\n';
	free (line);
	str[i] = '\0';
	return (str);
}

size_t	ft_strlen_gnl(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		i++;
		if (str[i] == '\n')
			return (i + 1);
	}
	return (i);
}

char	*ft_substr_gnl(char *buff, size_t start)
{
	size_t	i;
	char	*line;
	size_t	j;

	j = 0;
	i = 0;
	while (buff[start + j] != '\n' && buff[start + j])
		j++;
	if (buff[start + j] == '\n')
		j++;
	line = malloc(sizeof(char) * j + 1);
	if (!line)
		return (NULL);
	while (buff[start + i] != '\n' && buff[start + i])
	{
		line[i] = buff[start + i];
		i++;
	}
	if (buff[start + i] == '\n')
	{
		line[i] = '\n';
		i++;
	}
	line[i] = '\0';
	return (line);
}

int	check_error(t_struct *b, char *line)
{
	if (!line)
		return (1);
	if (check_line(line) == 1)
		return (2);
	if (b->bytes_read < BUFFER_SIZES)
		return (3);
	return (0);
}

char	*return_value(t_struct *b, char *line)
{
	if (check_error(b, line) == 1)
		return (NULL);
	if (check_error(b, line) == 2)
		return (b->pos = check_pos(b->buffer, 0) + 1, line);
	if (check_error(b, line) == 3)
	{
		b->pos = 0;
		b->bytes_read = 0;
		return (line);
	}
	return (0);
}
