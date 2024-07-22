CC = gcc

CFLAGS = -I./includes -I./libft -I/goinfre/jyap/.brew/opt/readline/include -fsanitize=address -g3

LDFLAGS = -L/goinfre/jyap/.brew/opt/readline/lib -lncurses -lreadline

OUTPUT = minishell

SRCS = $(wildcard *.c ./libft/*.c ./tokenization/*.c ./env/*.c ./execution/*.c)

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
