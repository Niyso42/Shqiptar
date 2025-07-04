NAME = minishell

SRC = src/minishell.c envi/get_env.c builtin/cd.c builtin/env.c builtin/echo.c builtin/export.c builtin/export_utils.c builtin/exit.c builtin/pwd.c builtin/unset.c builtin/is_builtin.c builtin/free_array.c parsing/token.c parsing/handle_prompt.c parsing/handle_cmd.c parsing/expand.c exec/exec.c exec/free.c exec/heredoc.c signals/signals.c 

OBJS = $(SRC:.c=.o)

FLAGS = -Wall -Wextra -Werror -g

CC = gcc

LIBS = -lreadline
LIBFT = libft/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(FLAGS) $(OBJS) $(LIBS) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C libft

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)
	make clean -C libft

fclean: clean
	rm -rf $(NAME)
	make fclean -C libft

re: fclean all

.PHONY: all clean fclean re