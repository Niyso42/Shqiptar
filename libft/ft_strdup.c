/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:31:51 by mubersan          #+#    #+#             */
/*   Updated: 2024/11/06 14:15:55 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		i;
	int		len;
	char	*strcpy;

	i = 0;
	len = 0;
	while (s[len])
	{
		len++;
	}
	strcpy = (char *)malloc(len + 1);
	if (!strcpy)
		return (NULL);
	while (s[i] != '\0')
	{
		strcpy[i] = s[i];
		i++;
	}
	strcpy[i] = '\0';
	return (strcpy);
}
/*
#include <string.h>

int main(void)
{
    char *original = NULL;
    char *duplicate;
	char *dup;

    duplicate = ft_strdup(original);
	dup = strdup(original);

    //printf("ft_strdup %s\n", duplicate);
	printf("strdup %s\n", dup);
    free(duplicate);
}*/