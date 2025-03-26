/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:18:46 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:13:42 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Check if the character is an alphabetic letter.
 * @param c (int) Character to check, passed as an unsigned char cast to int.
 * @return (int) 1 if the character is a letter, 0 otherwise.
 */
int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}
/*
#include <stdio.h>

int main()
{
	printf("%s\n%s\n%s\n%s\n%s\n%s\n",
		!ft_isalpha(' ')? "ok" : "fail",
		ft_isalpha('a')? "ok" : "fail",
		ft_isalpha('K')? "ok" : "fail",
		!ft_isalpha('8')? "ok" : "fail",
		!ft_isalpha('$')? "ok" : "fail",
		!ft_isalpha('\n')? "ok" : "fail");
}
*/