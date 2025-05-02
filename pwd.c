#include "bastien.h"

int	pwd_builtin(t_all *all)
{
	char	*path;

	path = getcwd(NULL, 0);//(cela permet de recuperer le repertoire courant)
	if (path == NULL)
	{
		perror("getcwd");
		all->exit_status = 1;
		return (1);
	}
	ft_printf("%s\n", path);
	return (0);
}
