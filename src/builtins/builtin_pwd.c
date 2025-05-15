/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sapupier <sapupier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:42:29 by sapupier          #+#    #+#             */
/*   Updated: 2025/05/15 18:42:30 by sapupier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd_builtin(t_all *all)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		perror("getcwd");
		all->exit_status = 1;
		return (1);
	}
	ft_printf("%s\n", path);
	free(path);
	return (0);
}
