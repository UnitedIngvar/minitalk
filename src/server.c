#include "../includes/minitalk.h"

static void sig_handler(int sig)
{
	static char counter;
	static char res_char;

	if (!counter)
		counter = 1;
	if (sig == SIGUSR1)
		res_char |= 1;
	if (counter == 7)
	{
		write (1, &res_char, 1);
		res_char = 0;
		counter = 0;
	}
	counter++;
	res_char <<= 1;
}

int	main(void)
{
	struct		sigaction act;
	sigset_t	set;
	pid_t		pid;

	pid = getpid();
	ft_putstr_fd("PID = ", 1);
	ft_putstr_fd(ft_itoa(pid));
	ft_putstr_fd("\n");
	act.sa_handler = sig_handler;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	act.sa_mask = set;
	sigaction(SIGUSR1, &act, 0);
	sigaction(SIGUSR2, &act, 0);
	while (1)
		pause();
}
