CC = cc
CFLAGS =   -Wall -Wextra -Werror
NAME = minishell
LIBFT = libft/libft.a

SRCM = main.c \
		executor/child_handler_multi.c  executor/executor.c  executor/handle_multi.c  executor/handle_single_child.c  executor/single_child_util_2.c  executor/single_child_utils.c \
		parsing/fill_executor_list.c  parsing/handle_signals.c  parsing/parser.c  parsing/prep_executor.c  parsing/syntax_cheker.c  parsing/tokenize.c \
		utils/clean_rl_copy.c  utils/environ_list_utils.c  utils/free_heap.c  utils/ft_newsplit.c  utils/ft_strcmp.c  utils/ft_strcpy.c  utils/grb_coll.c  utils/init_shell.c  utils/set_path_executor.c \
		builtins_fun/ft_cd.c  builtins_fun/ft_pwd.c  builtins_fun/handle_single_builtin.c builtins_fun/env_builtin.c builtins_fun/echo_builtin.c utils/ft_get_value.c utils/exit_status.c builtins_fun/export_builtin.c builtins_fun/unset_builtin.c builtins_fun/exit_builtin.c

SRCLIB = libft/ft_lstadd_back_bonus.c libft/ft_lstsize_bonus.c libft/ft_strjoin.c libft/ft_atoi.c libft/ft_lstadd_front_bonus.c \
			libft/ft_lstlast_bonus.c libft/ft_lstnew_bonus.c libft/ft_split.c libft/ft_strdup.c libft/ft_strlen.c libft/ft_isdigit.c

OBJS = $(SRCM:.c=.o)


all: $(LIBFT) $(NAME)

# $(NAME) : $(OBJS) 

$(NAME): $(OBJS) $(LIBFT) # add libft 
	$(CC) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

%.o: %.c includes/minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT): $(SRCLIB) libft/libft.h
	make -C libft/

clean:
	@rm -f $(OBJS)
	@make -C libft clean

fclean: clean
	@rm -f $(NAME)
	@make -C libft fclean

re: fclean all

.PHONY : clean