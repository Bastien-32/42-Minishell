/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 15:40:15 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:38:23 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Write a string followed by a newline to the given file descriptor.
 * @param s (char *) String to write.
 * @param fd (int) File descriptor to write to.
 */
void	ft_putendl_fd(char *s, int fd)
{
	while (*s)
		write(fd, s++, 1);
	write(fd, "\n", 1);
}
/*
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	char	*s = "toudou bein";
	int fd = open("output.txt", O_WRONLY | O_APPEND);
	if (fd == -1)
	{
		perror("Erreur d'ouverture du fichier");
		return (1);
	}
	ft_putendl_fd("\n", fd); 
	ft_putendl_fd(s, fd);
	ft_putendl_fd("\n", fd);
	close(fd);
	
	return (0);
}
*/