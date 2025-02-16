SRCS= parsing/main.c parsing/tokenizing.c parsing/lexing.c parsing/utils.c \
	parsing/counting_tokens.c parsing/expanding.c parsing/ft_split.c parsing/expanding_quotes.c garbage_collector.c
OBJS = $(SRCS:%.c=%.o)
HEADER = minishell.h

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