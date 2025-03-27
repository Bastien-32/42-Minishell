/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:40:47 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:22:44 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Delete and free all elements of a linked list.
 * @param lst (t_list **) Pointer to the list to clear.
 * @param del (void (*)(void *)) Function to delete the content of each node.
 */
void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list		*temp;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = temp;
	}
	*lst = NULL;
}
