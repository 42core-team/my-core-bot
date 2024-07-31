SRCDIR = src
OBJDIR = build
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))

CC = cc
CFLAGS = -Wall -Wextra -Werror -lm
COREDIR = /core
LIBS = $(COREDIR)/con_lib.a
INC = $(COREDIR)
EXEC = bot

PLAYER1_ID := 10
PLAYER2_ID := 20

.PHONY: run debug clean fclean re $(EXEC) stop

run: build $(EXEC)
	$(COREDIR)/game $(PLAYER1_ID) $(PLAYER2_ID) > /dev/null &
	$(COREDIR)/starlord $(PLAYER1_ID) > /dev/null &
	./$(EXEC) $(PLAYER2_ID)

debug: build $(EXEC)
	$(COREDIR)/game $(PLAYER1_ID) $(PLAYER2_ID) > /dev/null &
	$(COREDIR)/starlord $(PLAYER1_ID) > /dev/null &
	./$(EXEC) $(PLAYER2_ID)

battle: build $(EXEC)
	$(COREDIR)/game $(PLAYER1_ID) $(PLAYER2_ID) > /dev/null &
	../bot $(PLAYER1_ID) > /dev/null &
	./$(EXEC) $(PLAYER2_ID)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -L $(COREDIR) -l:con_lib.a -I $(INC)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC)

build: stop
	mkdir -p $(OBJDIR)

clean: stop
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(EXEC)

re: fclean run

stop:
	@pkill game > /dev/null || true &
	@pkill bot > /dev/null || true &
	@pkill starlord > /dev/null || true

update:
	@docker compose --project-directory=./.devcontainer pull
