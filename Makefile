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


# Devpod CLI executable
DEVCLI := ./devpod
# Default to vscode if none is specified
IDE ?= vscode

.PHONY: all dev-container

# Usage:
#   make devcontainer (IDE=<IDE>)
#
# This command sets up and launches a development container using the Devpod CLI.
# The IDE can be specified using the `IDE` environment variable (default: vscode).
# Ensure Docker is installed and running before executing this command.

# List of possible IDE values (for reference)
#   vscode, openvscode, cursor, zed, codium, intellij, pycharm, phpstorm,
#   rider, fleet, goland, webstorm, rustrover, rubymine, clion, dataspell,
#   jupyternotebook, vscode-insiders, positron, rstudio, web
#
devcontainer: __print-banner __install-devpod __check-docker __add-docker-provider __launch-devpod

# Define colors
COLOR_DATE :=\033[1;37m
COLOR_INFO :=\033[1;36m
COLOR_WARNING :=\033[1;33m
COLOR_ERROR :=\033[1;31m
COLOR_RESET :=\033[0m

__print-banner:
	@echo ""; \
	echo "$(COLOR_INFO)===================================================$(COLOR_RESET)"; \
	echo "$(COLOR_INFO)||                                               ||$(COLOR_RESET)"; \
	echo "$(COLOR_INFO)||           Welcome to your core bot!           ||$(COLOR_RESET)"; \
	echo "$(COLOR_INFO)||       Setting things up with Devpod CLI       ||$(COLOR_RESET)"; \
	echo "$(COLOR_INFO)||                 be ready ...                  ||$(COLOR_RESET)"; \
	echo "$(COLOR_INFO)===================================================$(COLOR_RESET)"; \
	echo "" ; \
	echo "$(COLOR_DATE)$$(date +'%H:%M:%S')$(COLOR_RESET) $(COLOR_INFO)info$(COLOR_RESET) 🚀 Starting Dev Container Environment"; \

__install-devpod:
	@if [ ! -f "$(DEVCLI)" ]; then \
		echo "$(COLOR_DATE)$$(date +'%H:%M:%S')$(COLOR_RESET) $(COLOR_INFO)info$(COLOR_RESET) Detecting OS and architecture..."; \
		OS_NAME=$$(uname -s | tr '[:upper:]' '[:lower:]'); \
		OS_ARCH=$$(uname -m); \
		if [ "$$OS_NAME" = "darwin" ]; then \
			if [ "$$OS_ARCH" = "arm64" ]; then \
				DEV_URL="https://github.com/loft-sh/devpod/releases/latest/download/devpod-darwin-arm64"; \
			elif [ "$$OS_ARCH" = "x86_64" ]; then \
				DEV_URL="https://github.com/loft-sh/devpod/releases/latest/download/devpod-darwin-amd64"; \
			else \
				echo "$(COLOR_DATE)$$(date +'%H:%M:%S')$(COLOR_RESET) $(COLOR_ERROR)error$(COLOR_RESET) Unsupported macOS architecture: $$OS_ARCH"; \
				exit 1; \
			fi; \
		elif [ "$$OS_NAME" = "linux" ]; then \
			if [ "$$OS_ARCH" = "x86_64" ]; then \
				DEV_URL="https://github.com/loft-sh/devpod/releases/latest/download/devpod-linux-amd64"; \
			elif [ "$$OS_ARCH" = "aarch64" ]; then \
				DEV_URL="https://github.com/loft-sh/devpod/releases/latest/download/devpod-linux-arm64"; \
			else \
				echo "$(COLOR_DATE)$$(date +'%H:%M:%S')$(COLOR_RESET) $(COLOR_ERROR)error$(COLOR_RESET) Unsupported Linux architecture: $$OS_ARCH"; \
				exit 1; \
			fi; \
		else \
			echo "$(COLOR_DATE)$$(date +'%H:%M:%S')$(COLOR_RESET) $(COLOR_ERROR)error$(COLOR_RESET) Unsupported OS: $$OS_NAME"; \
			exit 1; \
		fi; \
		echo "$(COLOR_DATE)$$(date +'%H:%M:%S')$(COLOR_RESET) $(COLOR_INFO)info$(COLOR_RESET) Installing Devpod CLI for OS='$$OS_NAME' ARCH='$$OS_ARCH'..."; \
		echo "$(COLOR_DATE)$$(date +'%H:%M:%S')$(COLOR_RESET) $(COLOR_INFO)info$(COLOR_RESET) Downloading from: $$DEV_URL"; \
		curl -L -o "$(DEVCLI)" "$$DEV_URL"; \
		chmod +x "$(DEVCLI)"; \
		echo "$(COLOR_DATE)$$(date +'%H:%M:%S')$(COLOR_RESET) $(COLOR_INFO)info$(COLOR_RESET) Devpod CLI installed in the current directory."; \
	else \
		echo "$(COLOR_DATE)$$(date +'%H:%M:%S')$(COLOR_RESET) $(COLOR_INFO)info$(COLOR_RESET) Devpod CLI is already installed."; \
	fi

__check-docker:
	@if ! docker info > /dev/null 2>&1; then \
		echo "$(COLOR_DATE)$$(date +'%H:%M:%S')$(COLOR_RESET) $(COLOR_ERROR)error$(COLOR_RESET) Docker is not running. Please start Docker and try again."; \
		exit 1; \
	fi; \
	echo "$(COLOR_DATE)$$(date +'%H:%M:%S')$(COLOR_RESET) $(COLOR_INFO)info$(COLOR_RESET) Docker is running."

__add-docker-provider:
	@if ./devpod provider add docker > /dev/null 2>&1; then \
		echo "$(COLOR_DATE)$$(date +'%H:%M:%S')$(COLOR_RESET) $(COLOR_INFO)info$(COLOR_RESET) Successfully added 'docker' as a provider."; \
	else \
		echo "$(COLOR_DATE)$$(date +'%H:%M:%S')$(COLOR_RESET) $(COLOR_WARNING)warning$(COLOR_RESET) The provider 'docker' already exists. If needed, run './devpod provider delete docker'."; \
	fi

__launch-devpod:
	@if [ "$(IDE)" = "web" ]; then \
		echo "$(COLOR_DATE)$$(date +'%H:%M:%S')$(COLOR_RESET) $(COLOR_INFO)info$(COLOR_RESET) Launching Devpod in web mode (no local IDE)."; \
		./devpod up .; \
	else \
		echo "$(COLOR_DATE)$$(date +'%H:%M:%S')$(COLOR_RESET) $(COLOR_INFO)info$(COLOR_RESET) Launching Devpod with IDE: $(IDE)"; \
		./devpod up . --ide "$(IDE)"; \
	fi

uninstall-devpod:
	@if [ -f "$(DEVCLI)" ]; then \
		echo "$(COLOR_DATE)$$(date +'%H:%M:%S')$(COLOR_RESET) $(COLOR_INFO)info$(COLOR_RESET) Uninstalling Devpod CLI..."; \
		rm -f "$(DEVCLI)"; \
		echo "$(COLOR_DATE)$$(date +'%H:%M:%S')$(COLOR_RESET) $(COLOR_INFO)info$(COLOR_RESET) Devpod CLI has been uninstalled."; \
	else \
		echo "$(COLOR_DATE)$$(date +'%H:%M:%S')$(COLOR_RESET) $(COLOR_INFO)info$(COLOR_RESET) Devpod CLI is not installed."; \
	fi