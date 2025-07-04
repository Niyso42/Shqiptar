/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:00:01 by mubersan          #+#    #+#             */
/*   Updated: 2024/11/06 14:37:04 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*dst;
	unsigned char	c2;
	size_t			i;

	c2 = (unsigned char) c;
	dst = (unsigned char *) s;
	i = 0;
	while (i < n)
	{
		dst[i] = c2;
		i++;
	}
	return (s);
}

/*#include <stdio.h>
#include <string.h>

int main(void)
{
	char *str = NULL;
	char *str2 = NULL;
	size_t n = 1;

	
	//printf("Avant memset : %s\n", str);

	ft_memset(str, 'A', n);
	//memset(str2, 'A', n);
	printf("Après memset : %s\n", str);
	//printf("Vraie fonction : %s", str2);
}*/
