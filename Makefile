SRC_DIR = src
BUILD_DIR = build
CORE_DIR = /core
INCLUDES = -I include -I /core
HEADERS = $(shell find include -name '*.h') $(shell find /core -name '*.h')
LIBS = /core/con_lib.a

SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

TARGET = bot
CXX = cc
CXXFLAGS = -Werror -Wall -Wextra $(INCLUDES)
LDFLAGS = $(LIBS)

PLAYER1_ID := 10
PLAYER2_ID := 20

run: $(TARGET)
	$(CORE_DIR)/game $(PLAYER1_ID) $(PLAYER2_ID) > /dev/null &
	$(CORE_DIR)/starlord $(PLAYER1_ID) > /dev/null &
	./$(TARGET) $(PLAYER2_ID)

debug: $(TARGET)
	./$(TARGET) $(PLAYER2_ID) > /dev/null &
	$(CORE_DIR)/starlord $(PLAYER1_ID) > /dev/null &
	$(CORE_DIR)/game $(PLAYER1_ID) $(PLAYER2_ID)

build: $(TARGET)

$(TARGET): stop $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean: stop
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -rf $(TARGET)

re: fclean run

stop:
	@pkill game > /dev/null || true &
	@pkill bot > /dev/null || true &
	@pkill starlord > /dev/null || true

update:
	@docker compose --project-directory=./.devcontainer pull

.PHONY: run debug battle $(TARGET) clean fclean re stop update
