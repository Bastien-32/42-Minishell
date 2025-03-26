/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:14:37 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:26:36 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Locate the first occurrence of a byte in a memory area.
 * @param s (const void *) Memory area to search.
 * @param c (int) Byte to find (interpreted as unsigned char).
 * @param n (size_t) Number of bytes to search.
 * @return (void *) Pointer to the matching byte, or NULL if not found.
 */
void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((unsigned char *)s)[i] == (unsigned char)c)
			return (&((unsigned char *)s)[i]);
		i++;
	}
	return (NULL);
}
/*
#include <stdio.h>

int main (void)
{
	int c = 'p';
	char *str = "ilop";
	char *d = ft_memchr(str, c, 4);
	printf("%p\n", d);
	printf("%p", &str[3]);
}
*/