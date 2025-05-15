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
