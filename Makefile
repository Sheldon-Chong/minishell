CC = gcc

CFLAGS = -I./includes -I./libft #-fsanitize=address -g3

LDFLAGS = -lncurses -lreadline

# CFLAGS = -I./includes -I./libft -I/goinfre/jyap/.brew/opt/readline/include -fsanitize=address -g3

# LDFLAGS = -lncurses -L/goinfre/jyap/.brew/opt/readline/lib -lreadline

OUTPUT = minishell

SRCS = $(wildcard src/*.c ./libft/*.c ./src/tokenization/*.c ./src/env/*.c ./src/execution/*.c ./src/execution/bash_cmd/*.c ./src/tokenization/*.c)

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

.PHONY: all clean fclean
