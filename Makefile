CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = libft.a

SRCM = main.c check_quote.c  clean_rl_copy.c  environ_list_utils.c  free_heap.c  handle_signals.c  init_shell.c 

OBJS = $(SRCM:.c=.o)


all: $(NAME)

$(NAME) : $(OBJS) 

%.o: %.c includes/minishell.h
	$(CC) $(CFLAGS) -c $<
	ar -r $(NAME) $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY : clean