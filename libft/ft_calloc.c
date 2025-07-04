/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:41:02 by mubersan          #+#    #+#             */
/*   Updated: 2024/11/04 17:19:23 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*c;
	size_t			i;

	i = 0;
	if (nmemb > SIZE_MAX / size)
		return (NULL);
	c = malloc(size * nmemb);
	if (!c)
	{
		return (NULL);
	}
	while (i < (size * nmemb))
	{
		c[i] = 0;
		i++;
	}
	return (c);
}
