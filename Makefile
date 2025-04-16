



NAME		= minishell
CC			= gcc

UNAME		:= $(shell uname)
ifeq ($(UNAME), Darwin)
	READLINE_FLAGS	= -I/opt/homebrew/opt/readline/include
	READLINE_LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline
else
	READLINE_FLAGS	=
	READLINE_LDFLAGS = -lreadline
endif

CFLAGS		= -g -Wall -Wextra -Werror -I. -I./libft $(READLINE_FLAGS)
LDFLAGS		= $(READLINE_LDFLAGS)
SRCS		= $(wildcard *.c)
OBJS		= $(SRCS:.c=.o)
LIBFT_DIR	= libft
LIBFT_A		= $(LIBFT_DIR)/libft.a

all: banner $(NAME) banner2

$(LIBFT_A):
	@make -s -C $(LIBFT_DIR)
	@echo ✅ "libft compiled"

$(NAME): $(LIBFT_A) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft $(LDFLAGS) -o $(NAME)
	@echo ✅ "Minishell compiled"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@make -s clean -C $(LIBFT_DIR)
	@echo "✅ Object files removed"

fclean: clean
	@rm -f $(NAME)
	@make -s fclean -C $(LIBFT_DIR)
	@echo "✅ Executable files removed"

re: fclean all

banner:
	@echo "\033[31m			         _         _       _            _  _ \033[0m"
	@echo "\033[31m			  /\\/\\  (_) _ __  (_) ___ | |__    ___ | || |\033[0m"
	@echo "\033[31m			 /    \\ | || '_ \\ | |/ __|| '_ \\  / _ \\| || |\033[0m"
	@echo "\033[31m			/ /\\/\\ \\| || | | || |\\__ \\| | | ||  __/| || |\033[0m"
	@echo "\033[31m			\\/    \\/|_||_| |_||_||___/|_| |_| \\___||_||_|\033[0m"

banner2:
	@echo "\033[31m⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣦⣀⠀⠀⠀⠀⠀⠀⢲⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡆⠀⠀⠀⠀⠀⠀⠀⠛⣦⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\033[0m"
	@echo "\033[31m⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣿⣷⣤⠀⠀⠀⠀⠀⢻⣿⣷⣄⢀⠀⠀⠀⠀⠀⠀⢀⣴⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢻⣿⣷⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\033[0m"
	@echo "\033[31m⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⣷⣄⠀⠀⠀⠀⣿⣿⣿⣷⠱⣆⠀⠀⠀⢀⣾⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\033[0m"
	@echo "\033[31m⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣽⣿⡆⠀⠀⠀⢸⣿⣞⣿⣧⢸⣷⣤⠀⢸⣿⣯⣿⠆⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⡟⢰⣿⡷⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⣿⣦⡄⠀⠀⠀⠀⢻⣿⣶⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\033[0m"
	@echo "\033[31m⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣟⣾⣿⠀⠀⠀⣼⣿⡏⣿⣿⠀⣿⣿⣗⠺⣿⣳⣿⣧⠀⠀⠀⠀⠀⠀⠀⣴⣿⡟⠀⣸⣿⡟⣽⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣆⠀⠀⠀⠀⣿⣿⣿⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\033[0m"
	@echo "\033[31m⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣦⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣯⢿⣿⡇⠀⣰⣿⡿⢸⣿⡿⠀⣼⣿⣻⡦⣿⣯⢿⣿⡆⠀⠀⠀⠀⢀⣾⣿⣿⠀⢠⣿⡿⢱⣿⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⢯⣿⠀⠀⢠⠀⣼⣿⣻⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\033[0m"
	@echo "\033[31m⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣷⡀⠀⠀⠀⠀⠀⣰⣿⣿⣞⣿⣿⠃⢀⣿⡿⣡⣿⡿⠃⢀⣿⣿⣽⣷⢹⣿⣻⢿⣿⡄⠀⠀⢀⣾⣿⢿⡇⠀⣾⣿⣱⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣿⣿⠁⢀⡿⢰⣿⣣⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\033[0m"
	@echo "\033[31m⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⡄⠀⠀⠀⢰⣿⣿⣻⣼⣿⡿⠀⢸⣿⢣⣿⡿⠁⣠⣿⣿⡟⣾⣿⢈⣿⣯⣟⣿⣷⠀⠀⣸⣿⣟⣿⡇⠀⣿⣧⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣳⣿⡟⠀⣾⡇⢸⣷⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀\033[0m"
	@echo "\033[31m⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⡄⠀⢠⣿⣿⢯⣳⣿⣿⣦⣄⠘⣿⣿⡿⠁⣴⣿⣿⢯⣽⣿⡟⢀⣿⣷⢯⣿⣿⡇⠀⣿⣿⣽⣻⣿⠀⣿⣷⣿⠇⠀⠀⠀⠀⣀⠀⠀⢀⣼⣿⣿⣳⢿⣿⠁⣰⣿⣇⢸⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⣠⡾⠁⠀⠀⠀⠀⠀⠀\033[0m"
	@echo "\033[31m⠀⠀⠀⠀⠀⠀⠀⢠⣾⠇⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⢿⣷⠀⣿⣿⢯⣟⣼⣿⡇⢻⣿⣧⠘⣿⠃⣼⣿⡿⡽⣞⣿⣿⠁⢸⣿⣟⣮⢿⣿⡇⠀⣿⣿⢶⣻⣿⣇⠘⣿⣿⠀⠀⢀⣴⡿⠁⠀⣠⣿⢱⣿⣯⣽⣻⣿⠀⣿⡿⣿⡄⢻⣧⠀⠀⠀⠀⠀⠀⣴⣾⡟⠁⠀⠀⠀⠀⠀⠀⠀\033[0m"


.PHONY: all clean fclean re banner
