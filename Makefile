SRCS= parsing/main.c parsing/tokenizing2.c parsing/lexing.c parsing/utils.c \
	
OBJS = $(SRCS:%.c=%.o)
HEADER = parsing/minishell.h

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -l readline -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) 

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean