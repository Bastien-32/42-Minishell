/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:52:42 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:25:07 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Create a new list by applying a function to each element's content.
 * @param lst (t_list *) Pointer to the first element of the original list.
 * @param f (void *(*)(void *)) Function to apply to each element's content.
 * @param del (void (*)(void *)) Function to delete content if allocation fails.
 * @return (t_list *) Pointer to the new list, or NULL if allocation fails.
 */
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*dest_list;
	t_list	*new_node;

	dest_list = NULL;
	while (lst)
	{
		new_node = ft_lstnew(f(lst->content));
		if (!new_node)
		{
			ft_lstclear(&dest_list, del);
			return (NULL);
		}
		ft_lstadd_back(&dest_list, new_node);
		lst = lst->next;
	}
	return (dest_list);
}

/*
void	*ft_upper_return(void *str)
{
	char	*ptr = ft_strdup((char *)str);
	char	*temp = ptr;

	if (!ptr)
		return (NULL);
	while (*temp)
	{
		if (*temp >= 'a' && *temp <= 'z')
			*temp -= 32;
		temp++;
	}
	return (ptr);
}

#include <stdio.h>

int main (void)
{
	char	*first_value = ft_strdup("Hello");
	char	*second_value = ft_strdup(" la");
	char	*last_value = ft_strdup(" team");

	t_list	*node0 = ft_lstnew(first_value);
	t_list	*node1 = ft_lstnew(second_value);
	t_list	*node2 = ft_lstnew(last_value);

	ft_lstadd_front(&node2, node1);
	ft_lstadd_front(&node2, node0);
	
	t_list	*new_list = ft_lstmap(node2, ft_upper_return, del);
	t_list	*temp = new_list;
	
	while (temp)
	{
		printf("%s", (char *)temp->content);
		temp = temp->next;
	}

	// Libère les deux listes
	ft_lstclear(&node2, del);
	ft_lstclear(&new_list, del);
	return 0;
}
*/