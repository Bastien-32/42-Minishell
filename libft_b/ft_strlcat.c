/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:34:54 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:46:25 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Append a string to another with size limit.
 * @param dst (char *) Destination buffer.
 * @param src (const char *) Source string to append.
 * @param dstsize (size_t) Total size of the destination buffer.
 * @return (size_t) Total length of the string it tried to create:
 *         initial length of dst plus length of src.
 */
size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	i = ft_strlen(dst);
	j = 0;
	if (size == 0 || size <= i)
		return (ft_strlen(src) + size);
	while ((j < (size - i - 1)) && src[j])
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = '\0';
	return (ft_strlen(src) + i);
}
/*
#include <stdio.h>

int	main (void)
{
	char *src = " la team";
	char dest[18] = "Bonjour";
	int i = ft_strlcat(dest, src, 12);
	printf("%s\n", dest);
	printf("%d", i);
	return (0);
}
*/