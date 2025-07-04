/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 14:05:24 by mubersan          #+#    #+#             */
/*   Updated: 2024/10/24 15:58:52 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;
	t_list	*suite;

	if (!lst || !del)
		return ;
	temp = *lst;
	while (temp)
	{
		suite = temp->next;
		del(temp->content);
		free(temp);
		temp = suite;
	}
	*lst = NULL;
}
