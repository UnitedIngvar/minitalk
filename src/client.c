#include "../includes/minitalk.h"

static int print_error_message()
{
	write(1, "Incorrect arguments usage;\nShold be: ./client [pid] [message]\n", 64);
	return (-1);
}

static void send_char(int c, int pid)
{
	int	i;

	i = 7;
	while (i >= 0)
	{
		if (c & (1 << i))
			kill(pid, SIGUSR2);
		else
			kill(pid, SIGUSR1);
		i--;
	}
}

static void process_string(char *str, pid_t pid)
{
	while (*str)
		send_char(*str, pid);
}

int mian(int argc, char **argv)
{
	if (!(argc == 3))
		return (print_error_message());
	process_string(argv[2], (pid_t)ft_atoi(argv[1]));
	while (1)
		pause();
}
