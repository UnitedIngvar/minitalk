#include "../includes/minitalk.h"

static void insert_bit(int bit_index, int *element, int signum)
{
	if (signum == ONE_BIT)
		*element |= bit_index;

	return (1);
}

static void insert_char(char **chunk, int signum)
{
	static int bit_index;
	static char c;

	if (bit_index == 0)
		bit_index = 1 << CHAR_BIT_LEN;
	insert_bit(bit_index, &c, signum);
	if (bit_index == 0)
	{
		**chunk = c;
		*chunk++;
		bit_index = 0;
	}
}

//TODO
static void clear_all(char *chunk, int *meta_bit_len_index)
{

}

static void chunk_handler(int signum, siginfo_t *siginfo, void *code)
{
	static int	chunk_len;
	static int	meta_bit_len_index;
	static int	bit_count;
	static char *chunk;

	if (meta_bit_len_index == 0)
		meta_bit_len_index = 1 << MSG_META_BIT_LEN;
	while (meta_bit_len_index != 0)
	{
		insert_bit(meta_bit_len_index, &chunk_len, signum);
		meta_bit_len_index >>= 1;
		bit_count++;
	}
	if (!chunk)
		chunk = ft_calloc(sizeof(char) * (chunk_len / CHAR_BIT_LEN + 1));
	while (chunk_len--)
	{
		insert_char(&chunk, signum);
		bit_count++;
	}
	while (chunk_len--)
	{
		insert_char(&chunk, ZERO_BIT);
		bit_count++;
	}
}

static void print_pid()
{
	pid_t pid;

	pid = getpid();
	ft_putstr_fd("PID = ", 1);
	ft_putstr_fd(ft_itoa(pid));
	ft_putstr_fd("\n");
}

int	main(void)
{
	struct		sigaction act;
	sigset_t	set;

	print_pid();
	act.sa_sigaction = chunk_handler;
	sigemptyset(&set);
	sigaddset(&set, ONE_BIT);
	sigaddset(&set, ZERO_BIT);
	act.sa_mask = set;
	sigaction(ONE_BIT, &act, 0);
	sigaction(ZERO_BIT, &act, 0);
	while (1)
		pause();
}
