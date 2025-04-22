#include "bastien.h"
#include<signal.h>
//sig_atomic_t	g_exit_status = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	//rl_redisplay();
	g_exit_status = 130;
}

void	handle_sigquit(int sig)
{
	(void)sig;
	// Ne rien faire (ignore Ctrl-\)
}

void	setup_signals_main(void)
{
	// struct sigaction sa;

	// sa.sa_handler = handle_sigint;
	// sa.sa_flags = SA_RESTART | SA_NODEFER;
	// sigemptyset(&sa.sa_mask);
	// sigaction(SIGINT, &sa, NULL);

	// // Le shell ignore SIGQUIT
	// sa.sa_handler = SIG_IGN;
	// sigaction(SIGQUIT, &sa, NULL);

	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = handle_sigint;
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);

	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);








}

void	setup_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
