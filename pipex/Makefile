
SRCS=		sources/error.c\
			sources/utils.c\
			sources/utils2.c\
			sources/main.c\
			sources/path_and_init.c\
			sources/split.c\

SRCS_BONUS=	sources_bonus/ft_push_b.c\
			sources_bonus/ft_rr.c\
			sources_bonus/ft_strncmp.c\
			sources_bonus/get_next_line.c\
			sources_bonus/ft_bubblesort.c\
			sources_bonus/ft_convert.c\
			sources_bonus/ft_exit.c\
			sources_bonus/ft_nodeadd_back.c\
			sources_bonus/ft_nodefirstremove.c\
			sources_bonus/ft_nodenew.c\
			sources_bonus/ft_reverserotate.c\
			sources_bonus/ft_rrr.c\
			sources_bonus/ft_swap.c\
			sources_bonus/get_next_line_utils.c\
			sources_bonus/ft_check.c\
			sources_bonus/ft_dispatch.c\
			sources_bonus/ft_intochained.c\
			sources_bonus/ft_nodeadd_front.c\
			sources_bonus/ft_nodelast.c\
			sources_bonus/ft_push_a.c\
			sources_bonus/ft_rotate.c\
			sources_bonus/ft_splitfree.c\
			sources_bonus/ft_testnum.c\
			sources_bonus/main.c



NAME=	pipex

NAME_BONUS=	pipex_bonus

CFLAGS=	-Werror -Wall -Wextra #-g

CC=	gcc

OBJS=	$(SRCS:.c=.o)

OBJS_BONUS=	$(SRCS_BONUS:.c=.o)

RM=	rm -f

all:	$(NAME)

bonus:	$(NAME_BONUS)

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(NAME_BONUS):	$(OBJS_BONUS)
				$(CC) $(CFLAGS) $(OBJS_BONUS) -o $(NAME_BONUS)
clean:
		$(RM) $(OBJS) $(OBJS_BONUS)
fclean:	clean
		$(RM) $(OBJS) $(NAME) $(NAME_BONUS)
		

re:	fclean all

.PHONY:	fclean clean re all