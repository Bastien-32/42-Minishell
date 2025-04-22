#include "bastien.h"
#include<signal.h>
//sig_atomic_t	g_exit_status = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_exit_status = 130;
}

void	handle_sigquit(int sig)
{
	(void)sig;
	// Ne rien faire (ignore Ctrl-\)
}

void	setup_signals_main(void)
{
	struct sigaction sa;
	// sigemptyset(&sa.sa_mask);
	// sa.sa_flags = 0;
	// sa.sa_handler = handle_sigint;
	// sigaction(SIGINT, &sa, NULL);
	// sa.sa_handler = SIG_IGN;
	// sigaction(SIGQUIT, &sa, NULL);
	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);

	// Le shell ignore SIGQUIT
	//signal(SIGQUIT, SIG_IGN);
}

void	setup_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
