/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:08:35 by mubersan          #+#    #+#             */
/*   Updated: 2024/10/22 18:52:36 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dst_len;
	size_t	src_len;

	i = 0;
	dst_len = 0;
	src_len = 0;
	while (*dst != '\0' && dst_len < size)
	{
		dst_len++;
		dst++;
	}
	while (src[src_len] != '\0')
		src_len++;
	if (size <= dst_len || size <= 0)
		return (src_len + size);
	while (src[i] != '\0' && dst_len + i + 1 < size)
	{
		*dst = src[i];
		dst++;
		i++;
	}
	*dst = '\0';
	return (dst_len + src_len);
}
/*
#include <stdio.h>

int	main(void)
{
	char			s[100] = "aaaa";
	char			d[100] = "ccccccc";
	unsigned int	size;

	size = 10;
	printf("%ld\n", ft_strlcat(d, s, size));
}
*/