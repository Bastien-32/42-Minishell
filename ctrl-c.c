#include "bastien.h"

/*  2->sigint  = ctrl-C
    3->sigquit = ctrl-\  */
void	handle_signals(void)
{
	signal(2, handle_sigint);
	signal(3, handle_sigquit);
}

void	handle_sigint(int sig)
{
	(void)sig;
	// write(STDOUT_FILENO, "\n", 1);
	// rl_replace_line("", 0);
	// rl_on_new_line();
	//rl_redisplay();
	// if (isatty(STDIN_FILENO))
	// {
	// 	rl_replace_line("", 0);
	// 	rl_on_new_line();
	// 	rl_redisplay(); // UNIQUEMENT si on est dans readline
	// }
	if (waitpid(-1, NULL, WNOHANG) == -1)
	{
		ft_printf("^C\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_sigquit(int sig)
{
	(void)sig;

	if (isatty(STDIN_FILENO) && g_exit_status != 0)
	{
		write (1, "Quit (core dumped)\n", 19);
		rl_redisplay();
	}
	//rl_redisplay();
}

void	setup_signals_main(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

// void	setup_signals_child(void)
// {
// }