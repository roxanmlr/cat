PROGRAM_NAME		=		bin/cat

CC					=		clang
SAN					=		-fsanitize=address,undefined
STD					=		-std=c17
LDFLAGS				=		-Wall -Wextra -Werror -g $(SAN) $(STD)
CFLAGS				=		-Wall -Wextra -Werror -g $(SAN) $(STD)


SRCS				=		cat.c
OBJS				=		$(patsubst %.c,build/%.o,$(SRCS))

ifeq ($(OS),Windows_NT)
	SHELL		= powershell.exe
	.SHELLFLAGS 	= -NoProfile -Command
	RM		= (Remove-Item -Recurse -Force -ErrorAction SilentlyContinue
	MKDIR		= (New-Item -ItemType Directory -Force -ErrorAction SilentlyContinue
	WIN_COMP	= ) -or (1 -eq 1)
	PROGRAM_NAME	= bin/cat.exe
	SAN		=
else
	SHELL		= /bin/bash
	RM		= rm -rf
	MKDIR		= mkdir -p
endif

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJS)
	$(MKDIR) bin $(WIN_COMP)
	$(CC) $(LDFLAGS) $^ -o $@

build/%.o: %.c
	$(MKDIR) build $(WIN_COMP)
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	$(RM) build $(WIN_COMP)

fclean: clean
	$(RM) $(PROGRAM_NAME) $(WIN_COMP)

re: fclean all

format:
	clang-format -i $(SRCS)

.PHONY: all clean fclean re format
