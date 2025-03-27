/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 10:33:57 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/27 18:46:21 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Duplicates up to n characters from a string.
 *
 * Allocates and returns a new string containing at most
 * n characters from the input string, null-terminated.
 *
 * @param s The source string.
 * @param n Maximum number of characters to duplicate.
 * @return A newly allocated string, or NULL on failure.
 */
char	*ft_strndup(const char *s, size_t n)
{
	size_t	i;
	char	*dup;

	dup = malloc(n + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (s[i] && i < n)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
