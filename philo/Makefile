CC        = gcc
CFLAGS    = -Wall -Wextra -Werror -pthread
INCLUDE   = -Iinclude

SRC_DIR   = src
CORE_DIR  = $(SRC_DIR)/core
UTILS_DIR = $(SRC_DIR)/utils
SYNC_DIR  = $(SRC_DIR)/synchronization
DATA_DIR  = $(SRC_DIR)/data

CORE_SRCS  = $(CORE_DIR)/main.c \
             $(CORE_DIR)/simulation.c \
             $(CORE_DIR)/threads.c \
             $(CORE_DIR)/routine.c \
             $(CORE_DIR)/extra.c
UTILS_SRCS = $(UTILS_DIR)/input_validation.c \
             $(UTILS_DIR)/time_utils.c \
             $(UTILS_DIR)/time_utils2.c \
             $(UTILS_DIR)/memory_utils.c \
             $(UTILS_DIR)/logging.c
SYNC_SRCS  = $(SYNC_DIR)/mutex_utils.c \
             $(SYNC_DIR)/philo_state.c
DATA_SRCS  = $(DATA_DIR)/forks.c \
             $(DATA_DIR)/meals.c

SRCS       = $(CORE_SRCS) $(UTILS_SRCS) $(SYNC_SRCS) $(DATA_SRCS)

OBJS       = $(SRCS:%.c=%.o)

NAME       = philo

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(shell tput setaf 2)[BUILD]$(shell tput sgr0) Linking ..."
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(shell tput setaf 2)[SUCCESS]$(shell tput sgr0) Build complete: $(NAME)"

%.o: %.c
	@echo "$(shell tput setaf 3)[COMPILE]$(shell tput sgr0) Compiling $<..."
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	@echo "$(shell tput setaf 5)[CLEAN]$(shell tput sgr0) Removing object files..."
	rm -f $(OBJS)

fclean: clean
	@echo "$(shell tput setaf 5)[FCLEAN]$(shell tput sgr0) Removing executable..."
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
