CC = gcc

CFLAGS = -I./includes -I./libft -fsanitize=address -g3

LDFLAGS = -lncurses -lreadline

OUTPUT = minishell

SRCS = $(wildcard *.c ./libft/*.c ./tokenization/*.c ./env/*.c ./execution/parse_cmd.c ./execution/parse_pipe_cmd.c ./execution/parse_cmd_utils.c ./execution/parse_pipe_cmd_utils.c)

OBJS = $(SRCS:%.c=%.o)

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c "$<" -o "$@"

clean:
	rm -f $(OBJS)

fclean:
	rm -f $(OBJS) $(OUTPUT)

.PHONY: all clean run
