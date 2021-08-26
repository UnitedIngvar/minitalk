#include "../includes/minitalk.h"

static void send_bit(pid_t pid, int bit)
{
	kill(pid, bit);
}

static void bit_sender(pid_t pid, signed short bit_len, signed short msg)
{
	int bit_index;

	int bit_index = 1;
	while (bit_len > 0)
	{
		if (msg & bit_index == 1)
			send_bit(pid, ONE_BIT);
		else
			send_bit(pid, ZERO_BIT);
		bit_len--;
		bit_index <<= 1;
	}
}

static char* get_chunk_bit_len(char *msg)
{
	int	max_len;
	int	payload_char_len;

	max_len = MSG_BIT_LEN / CHAR_BIT_LEN;
	payload_char_len = 0;
	while (max_len-- && msg++)
		payload_char_len++;
	return (payload_char_len * CHAR_BIT_LEN);
}

static void send_meta_info(pid_t pid, signed short payload_len)
{
	bit_sender(pid, MSG_META_BIT_LEN, payload_len);
}

static void send_char(pid_t pid, char c)
{
	bit_sender(pid, CHAR_BIT_LEN, c);
}

static void send_payload(pid_t pid, char *msg_offset, int payload_len)
{
	while (payload_len)
	{
		send_char(pid, msg_offset);
		msg_offset++;
		payload_len--;
	}
}

static void send_padding(pid_t pid, int padding_len)
{
	while (padding_len--)
		send_char(pid, '\0');
}

static void send_message(char *msg, pid_t pid)
{
	int	payload_len;
	int	padding_len;

	payload_len = 0;
	padding_len = 0;
	while (*msg)
	{
		//TODO: обработчк пустого сообщения
		payload_len = get_chunk_bit_len(msg);
		padding_len = MSG_BIT_LEN - (payload_len);
		send_meta_info(pid, payload_len);
		send_payload(pid, msg, payload_len);
		send_padding(pid, padding_len);
		msg += payload_len / CHAR_BIT_LEN;
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
