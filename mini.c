// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <signal.h>
// #include <sys/wait.h>

// void	setup_signals_child(void)
// {
// 	signal(SIGINT, SIG_DFL);
// 	signal(SIGQUIT, SIG_DFL);
// }

// void	wait_all_children(void)
// {
// 	int status;
// 	pid_t pid;
// 	while ((pid = wait(&status)) > 0)
// 	{
// 		if (WIFSIGNALED(status))
// 		{
// 			int sig = WTERMSIG(status);
// 			if (sig == SIGQUIT)
// 				write(2, "Quit (core dumped)\n", 20);
// 			else if (sig == SIGINT)
// 				write(2, "\n", 1);
// 		}
// 	}
// }

// int	main(void)
// {
// 	char *argv[] = { "sleep", "10", NULL };
// 	pid_t pid = fork();

// 	if (pid == 0)
// 	{
// 		setup_signals_child();
// 		execvp("sleep", argv);
// 		perror("execvp");
// 		exit(1);
// 	}
// 	else
// 	{
// 		wait_all_children();
// 	}
// 	return 0;
// }
