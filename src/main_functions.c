/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sapupier <sapupier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:45:09 by sapupier          #+#    #+#             */
/*   Updated: 2025/05/15 18:45:10 by sapupier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_noninteractive_input(void)
{
	char	*tmp_line;
	char	*line;

	tmp_line = get_next_line(fileno(stdin));
	if (tmp_line)
	{
		line = ft_strtrim(tmp_line, "\n");
		free(tmp_line);
	}
	else
		line = NULL;
	return (line);
}

void	exit_if_no_line(t_all *all, char *line)
{
	if (!line && !isatty(STDIN_FILENO))
		exit(0);
	if (!line && isatty(STDIN_FILENO))
	{
		free_env_list(all->env);
		free(all);
		write(1, "exit\n", 5);
		exit(0);
	}
}

t_all	*init_all(int argc, char **argv, char **envp)
{
	t_all	*all;

	(void)argc;
	(void)argv;
	all = malloc(sizeof(t_all));
	if (!all)
		return (NULL);
	all->env = init_env(envp);
	all->exit_status = 0;
	all->ast = NULL;
	all->lines = NULL;
	return (all);
}

void	exec_lines(t_all *all, char *line)
{
	int	i;

	all->lines = ft_split(line, '\n');
	i = 0;
	while (all->lines[i])
	{
		all->ast = ft_build_tree(all->lines[i], all);
		if (all->ast)
		{
			prepare_heredocs(all);
			execute_ast(all);
		}
		all->ast = NULL;
		i++;
	}
	free_tab(all->lines);
}
