#include "minishell.h"

volatile sig_atomic_t	g_sigint_received = 0;

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

char	*read_input(t_all *all)
{
	char	*line;

	rl_catch_signals = 0;
	if (g_sigint_received)
	{
		g_sigint_received = 0;
		all->exit_status = 0;
		return (NULL);
	}
	if (isatty(STDIN_FILENO))
		line = readline("minishell> ");
	else
		line = handle_noninteractive_input();
	exit_if_no_line(all, line);
	if (line[0] == '\0')
		return (free(line), NULL);
	if (*line && isatty(STDIN_FILENO))
		add_history(line);
	return (line);
}