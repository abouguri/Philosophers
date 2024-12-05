# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
INCLUDE = -Iinclude

# Directories
SRC_DIR = src
CORE_DIR = $(SRC_DIR)/core
UTILS_DIR = $(SRC_DIR)/utils
SYNC_DIR = $(SRC_DIR)/synchronization
DATA_DIR = $(SRC_DIR)/data
BUILD_DIR = build

# Source files
CORE_SRCS = $(CORE_DIR)/main.c $(CORE_DIR)/simulation.c $(CORE_DIR)/threads.c $(CORE_DIR)/routine.c $(CORE_DIR)/philos_count.c
UTILS_SRCS = $(UTILS_DIR)/input_validation.c $(UTILS_DIR)/time_utils.c $(UTILS_DIR)/memory_utils.c $(UTILS_DIR)/logging.c
SYNC_SRCS = $(SYNC_DIR)/mutex_utils.c $(SYNC_DIR)/philo_state.c
DATA_SRCS = $(DATA_DIR)/forks.c $(DATA_DIR)/meals.c

SRCS = $(CORE_SRCS) $(UTILS_SRCS) $(SYNC_SRCS) $(DATA_SRCS)

# Object files
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

# Output executable
NAME = philosophers

# Rules
all: $(NAME)

# Build the executable
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "Build complete: $(NAME)"

# Compile object files
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	@echo "Compiled: $< -> $@"

# Clean build files
clean:
	@rm -rf $(BUILD_DIR)
	@echo "Build directory cleaned."

# Clean everything, including the executable
fclean: clean
	@rm -f $(NAME)
	@echo "Executable removed."

# Rebuild the project
re: fclean all

# Phony targets
.PHONY: all clean fclean re
