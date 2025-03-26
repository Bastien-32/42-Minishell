/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:19:33 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:27:37 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Copy n bytes from source to destination memory areas.
 * @param dest (void *) Destination memory area.
 * @param src (const void *) Source memory area.
 * @param n (size_t) Number of bytes to copy.
 * @return (void *) Pointer to the destination memory area.
 */
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!dest && !src)
		return (dest);
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}
/*
#include <stdlib.h>
#include <stdio.h>

int	main (void)
{
	char src[]="bonjour";
	char *dest = malloc(20 * sizeof(char));

    if (dest == NULL) 
	{
        perror("malloc failed");
        return 1; 
	}
	printf("%s\n", src);
	memcpy(dest, src, 3);
	printf("%s\n", dest);
	ft_memcpy(dest, src, 3);
	printf("%s\n", dest);
	
}
*/