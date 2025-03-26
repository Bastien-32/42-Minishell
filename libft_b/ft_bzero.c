/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:18:05 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:10:15 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Set the first n bytes of the memory area pointed to by s to zero.
 * @param s (void *) Pointer to the memory area to be zeroed.
 * @param n (size_t) Number of bytes to set to zero.
 * @return (void *) The original pointer to the memory area.
 */
void	*ft_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*str;

	str = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		str[i] = 0;
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
	ft_bzero(str, i);
	str[i] = '\0';
	printf(" premier caractere : %c\n", str[0]);
	printf(" dernier caractere : %c\n", str[i - 1]);
	printf(" '\\0' : %c\n", str[i]);
}
*/