/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 15:35:00 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:42:19 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Duplicate a string by allocating sufficient memory.
 * @param s1 (const char *) String to duplicate.
 * @return (char *) Pointer to the newly allocated copy of the string,
 *         or NULL if allocation fails.
 */
char	*ft_strdup(const char *s)
{
	size_t	slen;
	char	*dest;

	slen = ft_strlen(s) + 1;
	dest = (char *)malloc((slen) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	ft_strlcpy(dest, s, slen);
	return (dest);
}
/* 
void	ft_putstr(char *str)
{
	while (*str)
		write(1, str++, 1);
}

int	main(void)
{
	char	src[] = "Bonjour";
	char	*dest;
	
	dest = ft_strdup(src);
	if (dest == NULL)
	{
		ft_putstr("Erreur d'allocation de memoire");
		return (1);
	}
	ft_putstr(dest);
	ft_putstr("\n");
	free(dest);
}
 */