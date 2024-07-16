CC = gcc

CFLAGS = -I./includes -I./libft -fsanitize=address -g3

LDFLAGS = -lncurses -lreadline

OUTPUT = minishell

SRCS = $(wildcard *.c ./libft/*.c ./tokenization/*.c ./env/*.c ./execution/*.c)

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
