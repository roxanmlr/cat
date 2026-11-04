PROGRAM_NAME		=		bin/cat

CC					=		clang
SAN					=		-fsanitize=address,undefined
STD					=		-std=c17
LDFLAGS				=		-Wall -Wextra -Werror -g $(SAN) $(STD)
CFLAGS				=		-Wall -Wextra -Werror -g $(SAN) $(STD)


SRCS				=		cat.c
OBJS				=		$(patsubst %.c,build/%.o,$(SRCS))

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJS)
	mkdir -p bin
	$(CC) $(LDFLAGS) $^ -o $@

build/%.o: %.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf build

fclean: clean
	rm -f $(PROGRAM_NAME)

re: fclean all

format:
	clang-format -i $(SRCS)

.PHONY: all clean fclean re format
