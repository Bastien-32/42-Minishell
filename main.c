/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 09:20:21 by student           #+#    #+#             */
/*   Updated: 2025/03/25 09:40:53 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int main(int argc, char **argv)
{
    int    i;
    
    i = argc;
    (void)argv;
    while (i < 10)
    {
        printf("i = %d\n", i++);
    }
    printf("C'est pas mal!\n");
    return (0);
}