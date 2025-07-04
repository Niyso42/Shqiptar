/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:59:47 by mubersan          #+#    #+#             */
/*   Updated: 2024/11/06 14:17:32 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}

// #include <stdio.h>
// #include <strings.h>

// int main(void)
// {
// 	char str[50] = "Bonjour, voici une chaîne de test !";
// 	char str2[50] = "Bonjour, voici une chaîne de test !";
// 	printf("Avant bzero : %s\n", str);

// 	ft_bzero(str, 7);
// 	bzero(str2, 7);
// 	printf("Après bzero : %s\n", str);
// 	printf("Vraie fonction : %s", str2);

// }