SRCS = building_command.c errors.c ft_split.c main.c utils.c
OBJS = $(SRCS:%.c=%.o)
HEADER = pipex.h

B_SRCS = bonus/building_command_bonus.c bonus/errors_bonus.c bonus/ft_split_bonus.c \
         bonus/pipex_bonus.c bonus/utils_bonus.c bonus/get_next_line_bonus.c bonus/here_doc_bonus.c
B_OBJS = $(B_SRCS:%.c=%.o)
B_HEADER = bonus/pipex_bonus.h

NAME = pipex
CC = cc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

bonus: $(B_OBJS) $(B_HEADER)
	$(CC) $(CFLAGS) $(B_OBJS) -o $(NAME)

bonus/%.o: bonus/%.c $(B_HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(B_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean