LIBC = ft_atoi.c ft_atol.c ft_bzero.c ft_calloc.c ft_isalnum.c \
		ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c \
		ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c \
		ft_memset.c ft_strchr.c ft_strdup.c ft_strlcat.c \
		ft_strlcpy.c ft_strlen.c ft_strncmp.c ft_strnstr.c \
		ft_strrchr.c ft_tolower.c ft_toupper.c ft_substr.c \
		ft_strtrim.c ft_split.c ft_strjoin.c ft_itoa.c \
		ft_strmapi.c ft_striteri.c ft_putstr_fd.c ft_putendl_fd.c \
		ft_putnbr_fd.c ft_putchar_fd.c ft_strndup.c ft_strcmp.c\
		get_next_line_bonus.c get_next_line_utils_bonus.c \
		ft_printf.c ft_strlen_pf.c\
		ft_putnbr_addr_ptr_pf.c ft_address_pf.c ft_putnbr_base_pf.c\
		ft_putchar_pf.c ft_putnbr_pos_pf.c ft_putnbr_pf.c ft_putstr_pf.c \
		ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
		ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c \
		ft_lstmap.c 

SRC = ${LIBC}

OBJS = ${SRC:.c=.o}


NAME = libft.a 

CC = gcc

FLAGS = -Wall -Werror -Wextra

%.o: %.c
	${CC} ${FLAGS} -c $< -o $@

${NAME}: ${OBJS}
	ar -rsc ${NAME} ${OBJS}

all: ${NAME}

clean:
	rm -f ${OBJS}

fclean: clean;
	rm -f ${NAME}

re:	fclean all

.PHONY: all clean fclean re bonus