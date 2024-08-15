CC = cc

CFLAGS = -I./includes -I./libft -Wall -Wextra -Werror #-fsanitize=address -g3

LDFLAGS = -lncurses -lreadline

#CFLAGS = -I./includes -I./libft -I/goinfre/jyap/.brew/opt/readline/include -fsanitize=address -g3

#LDFLAGS = -lncurses -L/goinfre/jyap/.brew/opt/readline/lib -lreadline

OUTPUT = minishell

SRCS = $(wildcard src/*.c ./libft/*.c ./src/tokenization/*.c ./src/env/*.c ./src/execution/*.c ./src/execution/builtins/*.c ./src/tokenization/*.c ./src/signals/*.c ./src/chunking/*.c ./src/tokenization/quotes/*.c ./src/tokenization/expand_env/*.c)

OBJ_DIR = obj

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c "$<" -o "$@"

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(OUTPUT)

re: fclean all

.PHONY: all clean fclean re
