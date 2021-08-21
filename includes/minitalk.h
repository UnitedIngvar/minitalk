#ifndef MINITALK_H
# define MINITALK_H

# include <signal.h>
# include <unistd.h>
# include <stdlib.h>

int		ft_atoi(char const *str);
void	ft_putnbr(int nb);

# define MSG_BIT_LEN 4096
# define MSG_META_BIT_LEN 16
# define MSG_PAYLOAD_LEN (MSG_BYTE_LEN - MSG_META_BYTE_LEN)
# define CHAR_BIT_LEN 8
# define ZERO_BIT SIGUSR1
# define ONE_BIT SIGUSR2
#endif
