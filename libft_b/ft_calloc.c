/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 15:58:24 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:12:29 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Allocate memory for an array and initialize it to zero.
 * @param nmemb (size_t) Number of elements to allocate.
 * @param size (size_t) Size of each element.
 * @return (void *) Pointer to the allocated and zero-initialized memory,
 *         or NULL if allocation fails.
 */
void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	slen;
	char	*dest;

	slen = nmemb * size;
	dest = (char *)malloc((slen) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	ft_bzero(dest, slen);
	return (dest);
}
/*
void	ft_putstr(char *str)
{
	while (*str)
		write(1, str++, 1);
}

int	main(void)
{
	char	*dest;

	dest = ft_calloc(6, 3);
	if (dest == NULL)
	{
		ft_putstr("Erreur d'allocation de memoire");
		return (1);
	}
	ft_putstr(dest);
	ft_putstr("\n");
	free(dest);
}
*/