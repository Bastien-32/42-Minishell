/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:18:23 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:12:59 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Check if the character is alphanumeric (letter or digit).
 * @param c (int) Character to check, passed as an unsigned char cast to int.
 * @return (int) 1 if the character is alphanumeric, 0 otherwise.
 */
int	ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9'))
		return (1);
	else
		return (0);
}
/*
#include <stdio.h>

int main()
{
	printf("%s\n%s\n%s\n%s\n%s\n%s\n",
		!ft_isalnum(' ')? "ok" : "fail",
		ft_isalnum('a')? "ok" : "fail",
		ft_isalnum('K')? "ok" : "fail",
		ft_isalnum('8')? "ok" : "fail",
		!ft_isalnum('$')? "ok" : "fail",
		!ft_isalnum('\n')? "ok" : "fail");
}
*/