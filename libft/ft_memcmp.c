/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:59:55 by mubersan          #+#    #+#             */
/*   Updated: 2024/11/04 17:08:16 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*s11;
	unsigned char	*s22;

	s11 = (unsigned char *)s1;
	s22 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (s11[i] != s22[i])
			return (s11[i] - s22[i]);
		i++;
	}
	return (0);
}
/*
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char *str1 = NULL;
	char *str2 = NULL;
	char str3[] = "Hello, World!";
	//printf("%d\n", ft_memcmp(str1, str2, 5));
	//printf("%d\n", ft_memcmp(str2, str3, 15));
	printf("%d\n", ft_memcmp(str1, str3, 6));
	//printf("%d\n", memcmp(str1, str2, 0));
	//printf("%d\n", memcmp(str2, str3, 15));
	//printf("%d\n", memcmp(str1, str3, 6));
	return (0);
}*/