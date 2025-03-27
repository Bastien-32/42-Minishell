/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:05:47 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:45:11 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*
char	ft_upper(unsigned int i, char c)
{
	(void) i;
	if (c >= 'a' && c <= 'z')
		return (c -= 32);
	else
		return (c);
}
*/

/**
 * @brief Apply a function to each character of a string to create a new string.
 * @param s (const char *) Input string.
 * @param f (char (*)(unsigned int, char)) Function applied to each character.
 * @return (char *) Newly allocated string resulting from successive applications
 *         of f, or NULL if allocation fails.
 */
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*str;

	i = 0;
	str = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (s[i])
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}

/*
#include <stdio.h>

int main (void)
{
	char *str = "Hello la compagnie 42!";
	char *dest;
	dest = ft_strmapi(str, ft_upper);
	if (dest)
	{
		printf("%s\n", dest);
		free(dest);
	}
}
*/