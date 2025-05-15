/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sapupier <sapupier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:42:10 by sapupier          #+#    #+#             */
/*   Updated: 2025/05/15 18:42:12 by sapupier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_builtin(t_ast *node, t_all *all)
{
	t_env	*tmp_env;

	tmp_env = all->env;
	if (node->cmd[1])
	{
		write(2, "env: too many arguments\n", 25);
		all->exit_status = 1;
		return (1);
	}
	while (tmp_env)
	{
		if (tmp_env->value != NULL)
			ft_printf("%s=%s\n", tmp_env->env_keyname, tmp_env->value);
		tmp_env = tmp_env->next;
	}
	return (0);
}
