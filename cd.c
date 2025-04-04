#include "bastien.h"

int	cd_builtin(char **args)
{
	char	*dir;

	if (args[1] == NULL)
	{
		dir = getenv("HOME");
		if (dir == NULL)
		{
			ft_printf("cd: HOME not set\n");
			return (1);
		}
	}
	else
		dir = args[1];
	if (chdir(dir) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}