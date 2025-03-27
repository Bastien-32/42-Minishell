/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 16:00:56 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:37:52 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Write a character to the given file descriptor.
 * @param c (char) Character to write.
 * @param fd (int) File descriptor to write to.
 */
void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
/*
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int fd = open("output.txt", O_WRONLY | O_APPEND);
	if (fd == -1)
	{
		perror("Erreur d'ouverture du fichier");
		return (1);
	}
	ft_putchar_fd('\n', 1); 
	ft_putchar_fd('C', fd);
	ft_putchar_fd('\n', fd);
	close(fd);
	
	return (0);
}
*/