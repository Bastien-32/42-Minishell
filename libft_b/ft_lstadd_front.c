/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 09:35:23 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:22:10 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Add a new element at the beginning of a linked list.
 * @param lst (t_list **) Pointer to the first element of the list.
 * @param new (t_list *) New element to add at the beginning.
 */
void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}
/*
#include <stdio.h>

int main (void)
{
	char	*last_value;
	char	*second_value;
	char	*first_value;

	first_value = "Hello";
	second_value = " la";
	last_value = " team";
	t_list	*node0;
	t_list	*node1;
	t_list	*node2;
	node0 = ft_lstnew(first_value);
	node1 = ft_lstnew(second_value);
	node2 = ft_lstnew(last_value);
	ft_lstadd_front(&node2, node1);
	ft_lstadd_front(&node2, node0);
	while (node2)
	{
		printf("%s", (char *)node2->content);
		node2 = node2->next;
	}
	
}
*/