/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sapupier <sapupier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 07:51:29 by sapupier          #+#    #+#             */
/*   Updated: 2025/03/26 15:53:24 by sapupier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "libft.h"
// Permet de chercher la 1ere occurence d un caractere dans une chaine
char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	if ((char)c == '\0')
		return ((char *)&s[i]);
	return (NULL);
}
/*
int	main(void)
{
	printf("%s\n", ft_strchr("Bonjour", 'o'));
	printf("%p\n", strchr("Bonjour", 'o'));
}*/