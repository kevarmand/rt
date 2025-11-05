# === Config ===
NAME        := bvh_test
CC          := cc
CFLAGS      := -Wall -Wextra -MMD -MP -lm -O2 -ffast-math -march=native
INCS        := -I. -I../../includes
LIBS        := ../../lib/libft/libft.a

# === Sources ===
SRCS        := $(wildcard *.c)
OBJS        := $(SRCS:.c=.o)
DEPS        := $(OBJS:.o=.d)

# === Règles ===
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

-include $(DEPS)

clean:
	$(RM) $(OBJS) $(DEPS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
