/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 09:48:01 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:51:23 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_empty_s(void)
{
	char	*dest;

	dest = (char *)malloc(1);
	if (!dest)
		return (NULL);
	dest[0] = '\0';
	return (dest);
}

/**
 * @brief Extract a substring from a string.
 * @param s (char const *) Original string.
 * @param start (unsigned int) Starting index of the substring.
 * @param len (size_t) Maximum length of the substring.
 * @return (char *) Newly allocated substring, or NULL if allocation fails.
 */
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*dest;

	i = 0;
	if (!s)
		return (NULL);
	if ((size_t)start >= ft_strlen(s))
		return (ft_empty_s());
	if (len > ft_strlen(s) || (len > ft_strlen(s) - start))
		len = ft_strlen(s) - start;
	dest = (char *)malloc((len + 1) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	while (((unsigned char *)s)[start + i] && i < len)
	{
		dest[i] = ((unsigned char *)s)[start + i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

/*
#include <stdio.h>

int main (void)
{
	char *dest;
	char *src = "0123456789";
	
	dest = ft_substr(src, 9, 10);
	printf("%s\n", dest);
	free(dest);
}
*/

/* bornes a gerer :
	start > strlen(s) 
		=> retourner \0 en faisant un malloc de 1 
			en debut de fonction 
	len > ft_strlen(s) || (start + len > ft_strlen(s))
		=> sinon alloue trop de memoire
*/
