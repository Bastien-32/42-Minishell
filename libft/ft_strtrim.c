/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:14:57 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:48:13 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Remove all characters in set from the beginning and end of a string.
 * @param s1 (char const *) String to trim.
 * @param set (char const *) Set of characters to remove.
 * @return (char *) Newly allocated trimmed string, or NULL if allocation fails.
 */
int	ft_strchrtrim(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	start;
	size_t	end;
	char	*dest;

	i = 0;
	start = 0;
	end = ft_strlen(s1);
	while (s1[start] && ft_strchrtrim(set, s1[start]))
		start++;
	while (end > start && ft_strchrtrim(set, s1[end - 1]))
		end--;
	dest = (char *)malloc((end - start + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	while (i < end - start)
	{
		dest[i] = s1[start + i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
/*
#include <stdio.h>

int main(void)
{
	char	*s = "   Hello, world   !   ";
	char	*set = " ";
	char	*dest = ft_strtrim(s, set);
	printf("%s\n", dest);
	free(dest);
}
*/