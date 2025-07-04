/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:33:44 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/06/17 14:56:47 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strdup_until(const char *s, char c)
{
	int		len;
	char	*dup;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	ft_strlcpy(dup, s, len + 1);
	return (dup);
}

char	**ft_strdup_array(char **array)
{
	int		i;
	char	**copy;

	if (!array)
		return (NULL);
	i = 0;
	while (array[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (array[i])
	{
		copy[i] = ft_strdup(array[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

void	ft_sort_ascii(char **array)
{
	int		i;
	int		j;
	char	*tmp;

	for (i = 0; array[i]; i++)
	{
		for (j = i + 1; array[j]; j++)
		{
			if (ft_strcmp(array[i], array[j]) > 0)
			{
				tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
			}
		}
	}
}

char	**ft_add_to_array(char **array, const char *new_str)
{
	char	**new_array;
	int		i;
	int		len;

	len = 0;
	while (array && array[len])
		len++;
	new_array = malloc(sizeof(char *) * (len + 2));
	if (!new_array)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_array[i] = array[i];
		i++;
	}
	new_array[i] = strdup(new_str);
	if (!new_array[i])
	{
		free(new_array);
		return (NULL);
	}
	new_array[i + 1] = NULL;
	free(array);
	return (new_array);
}

void	ft_puterror(const char *prefix, const char *arg, const char *msg)
{
	write(2, prefix, strlen(prefix));
	write(2, arg, strlen(arg));
	write(2, msg, strlen(msg));
	write(2, "\n", 1);
}