/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:19:12 by badal-la          #+#    #+#             */
/*   Updated: 2025/03/21 10:14:52 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Check if the character is a digit (0 to 9).
 * @param c (int) Character to check.
 * @return (int) 1 if the character is a digit, 0 otherwise.
 */
int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}
/*
#include <stdio.h>

int main()
{
	printf("%s\n%s\n%s\n%s\n%s\n%s\n",
		!ft_isdigit(' ')? "ok" : "fail",
		!ft_isdigit('a')? "ok" : "fail",
		!ft_isdigit('K')? "ok" : "fail",
		ft_isdigit('8')? "ok" : "fail",
		!ft_isdigit('$')? "ok" : "fail",
		!ft_isdigit('\n')? "ok" : "fail");
}
*/