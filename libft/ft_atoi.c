/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:52:39 by mubersan          #+#    #+#             */
/*   Updated: 2024/11/06 14:13:48 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int			i;
	int			sign;
	long int	nb;

	i = 0;
	nb = 0;
	sign = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		if ((sign == 1 && nb > (LONG_MAX - (nptr[i] - '0')) / 10))
			return (-1);
		if ((sign == -1 && nb > (LONG_MIN + (nptr[i] - '0')) / -10))
			return (0);
		nb = nb * 10 + (nptr[i] - '0');
		i++;
	}
	return (nb * sign);
}
/*
#include <stdio.h>
#include <string.h>

int	main(void)
{
	char	str[] = "-55855556564654646464654654585885";
	char	*c;

	c = NULL;
	printf("ft_atoi: %d\n", ft_atoi(str));
	printf("vrai function: %d\n", atoi(str));
}*/
