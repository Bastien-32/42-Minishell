#include "bastien.h"

int	pwd_builtin(void)
{
	char	*path;

	path = getcwd(NULL, 0);//(cela permet de recuperer le repertoire courant)
	if (path == NULL)
	{
		perror("getcwd");
		return (1);
	}
	ft_printf("%s\n", path);
	free (path);
	return (0);
}
