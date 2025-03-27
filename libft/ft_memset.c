/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:19:58 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:28:43 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Fill a memory area with a constant byte.
 * @param s (void *) Pointer to the memory area to fill.
 * @param c (int) Byte value to set (interpreted as unsigned char).
 * @param n (size_t) Number of bytes to set.
 * @return (void *) Pointer to the memory area.
 */
void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)s)[i] = (unsigned char)c;
		i++;
	}
	return (s);
}
/*
#include <stdio.h>

int main()
{
	int		i;
	char	*str;
	
	i = 4;
	str = (char *)malloc(i + 1);
	if (str == NULL)
		return (1);
	ft_memset(str, 'i', i);
	str[i] = '\0';
	printf(" premier caractere : %c\n", str[0]);
	printf(" dernier caractere : %c\n", str[i - 1]);
	printf(" '\\0' : %c\n", str[i]);
}
*/