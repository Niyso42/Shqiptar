/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:59:59 by mubersan          #+#    #+#             */
/*   Updated: 2024/11/06 14:16:56 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*dst;
	unsigned char	*srce;
	size_t			i;

	dst = (unsigned char *)dest;
	srce = (unsigned char *)src;
	i = 0;
	if (dst > srce)
	{
		i = n;
		while (i > 0)
		{
			i--;
			dst[i] = srce[i];
		}
	}
	else
	{
		ft_memcpy(dest, src, n);
	}
	return (dst);
}
/*
#include <stdio.h>
#include <string.h>

int main(void)
{
    char src[] = "Je suis";
   // char *dest = NULL;
	char dest2[] = "test";
	size_t n = 3;

	//ft_memmove(dest, src, n);
	//printf("Après memmove custom: dest = \"%s\"\n", dest);
    ft_memmove(dest2, src, n);
   printf("Après memmove standard: dest =  %s\n", dest2);
}*/