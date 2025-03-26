/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:07:21 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:24:32 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Return the last element of a linked list.
 * @param lst (t_list *) Pointer to the first element of the list.
 * @return (t_list *) Pointer to the last element, or NULL if the list is empty.
 */
t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

/*
#include <stdio.h>

int main (void)
{
	char	*last_value;
	char	*second_value;
	char	*first_value;
	t_list	*test;

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
	test = ft_lstlast(node2);
	printf("%s", (char *)test->content);	
}
*/
