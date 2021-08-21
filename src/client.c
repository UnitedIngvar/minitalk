#include "../includes/minitalk.h"

static void send_bit(pid_t pid, int byte)
{
	kill(pid, byte);
}

static void byte_sender(pid_t pid, signed short byte_len, signed short msg)
{
	int byte_index;

	int byte_index = 1;
	while (byte_len > 0)
	{
		if (msg & byte_index == 1)
			send_bit(pid, ONE_BIT);
		else
			send_bit(pid, ZERO_BIT);
		byte_len--;
		byte_index <<= 1;
	}
}

static char* get_chunk_len(char *msg)
{
	int	max_len;
	int	chunk_len;

	max_len = MSG_BIT_LEN;
	chunk_len = 0;
	while (max_len-- && msg++)
		chunk_len++;
	return (chunk_len);
}

static void send_meta_info(pid_t pid, signed short chunk_len)
{
	byte_sender(pid, MSG_META_BIT_LEN, chunk_len);
}

static void send_char(pid_t pid, char c)
{
	byte_sender(pid, CHAR_BIT_LEN, c);
}

static void send_chunk(pid_t pid, char *msg_offset, int chunk_len)
{
	while (chunk_len)
	{
		send_char(pid, msg_offset);
		msg_offset++;
		chunk_len--;
	}
}

static void send_padding(pid_t pid, int padding_len)
{
	while (padding_len--)
		send_char(pid, '\0');
}

static void send_message(char *msg, pid_t pid)
{
	int	chunk_len;
	int	padding_len;

	chunk_len = 0;
	padding_len = 0;
	while (*msg)
	{
		//TODO: обработчк пустого сообщения
		chunk_len = get_chunk_len(msg);
		padding_len = MSG_BIT_LEN - (chunk_len * CHAR_BIT_LEN);
		send_meta_info(pid, chunk_len);
		send_chunk(pid, msg, chunk_len);
		send_padding(pid, padding_len);
		msg += chunk_len;
	}
}

int mian(int argc, char **argv)
{
	char	*msg;
	pid_t	pid;

	if (!(argc == 3))
	{
		write(2, "Incorrect arguments usage;\nShold be: ./client [pid] [message]\n", 64);
		return (-1);
	}
	pid = (pid_t)ft_atoi(argv[1]);
	send_message(argv[2], pid);
	while (1)
		pause();
}
