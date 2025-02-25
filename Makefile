SRCS= main.c parsing/tokenizing.c parsing/lexing.c parsing/utils.c parsing/ft_itoa.c \
	parsing/counting_tokens.c parsing/expanding.c parsing/ft_split.c parsing/expanding_quotes.c \
	garbage_collector.c parsing/parsing.c execution/builtins.c execution/command.c  \
	execution/path.c execution/utils.c execution/infile_outfile.c execution/pipe.c execution/export.c
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