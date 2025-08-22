SRCDIR = src
OBJDIR = build
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))

CC = cc
CFLAGS = -Wall -Wextra -Werror -lm -g -fsanitize=address
CONNECTIONDIR ?= /core
LIBS = $(CONNECTIONDIR)/core_lib.a
INC = -I inc -I $(CONNECTIONDIR)/inc
EXEC = gridmaster
.PHONY: all run clean fclean re $(EXEC)

all: build $(EXEC)

run: all
	./$(EXEC)

$(EXEC): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ -L $(CONNECTIONDIR) $(INC) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

build:
	make -C $(CONNECTIONDIR)
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(EXEC)

re: fclean all
