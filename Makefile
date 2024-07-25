CC = gcc

CFLAGS = -I./includes -I./libft #-fsanitize=address -g3

LDFLAGS = -lncurses -lreadline

OUTPUT = minishell

SRCS = $(wildcard *.c ./libft/*.c ./tokenization/*.c ./env/*.c ./execution/*.c ./execution/bash_cmd/*.c)

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
