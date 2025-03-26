/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:17:46 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:47:07 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Copy a string to another with size limit.
 * @param dst (char *) Destination buffer.
 * @param src (const char *) Source string to copy.
 * @param dstsize (size_t) Total size of the destination buffer.
 * @return (size_t) Total length of the string it tried to create:
 *         initial length of dst plus length of src.
 */
size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (src[i] && i < size - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}
/*
#include <stdio.h>

int	main (void)
{
	char *src = "Bonjour";
	char dest[5];
	ft_strlcpy(dest, src, 5);
	printf("%s", dest);
	return (0);
}
*/