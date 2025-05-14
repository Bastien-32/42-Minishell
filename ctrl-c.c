#include "bastien.h"

void	handle_sigint(int sig)
{
	(void)sig;
	if (g_sigint_received == 1 && sig == SIGINT)
	{
		ft_putstr_fd("^C", 1);
		rl_done = 1;
		g_sigint_received = SIGINT;
	}
	else if (sig == SIGINT && g_sigint_received != 1)
	{
		if (waitpid(-1, NULL, WNOHANG) == -1)
		{
			ft_putstr_fd("^C\n", 1);
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
		else
		{
			ft_putstr_fd("\n", 1);
		}
		g_sigint_received = SIGINT;
	}
}

void	handle_sigquit(int sig)
{
	(void)sig;
	if (waitpid(-1, NULL, WNOHANG) == -1)
		return ;
	else
		ft_putstr_fd("Quit (core dumped)\n", 1);
}

void	setup_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	setup_signals_parent(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}
