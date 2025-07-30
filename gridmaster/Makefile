SRCDIR = src
OBJDIR = build
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))

CC = cc
CFLAGS = -Wall -Wextra -Werror -lm -g -fsanitize=address
CONNECTIONDIR = /core
LIBS = $(CONNECTIONDIR)/con_lib.a
INC = $(CONNECTIONDIR)
EXEC = gridmaster
.PHONY: all run clean fclean re $(EXEC)

all: build $(EXEC)

run: all
	./$(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -L $(CONNECTIONDIR) -I $(INC) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC)

build:
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(EXEC)

re: fclean all
