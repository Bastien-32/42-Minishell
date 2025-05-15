/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sapupier <sapupier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:45:17 by sapupier          #+#    #+#             */
/*   Updated: 2025/05/15 18:45:18 by sapupier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_all	*all;
	char	*line;

	all = init_all(argc, argv, envp);
	if (!all)
		return (1);
	while (1)
	{
		setup_signals_parent();
		line = read_input(all);
		if (!line)
			continue ;
		exec_lines(all, line);
		free(line);
	}
	free_env_list(all->env);
	free(all);
	return (0);
}
