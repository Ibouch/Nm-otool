# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ibouchla <ibouchla@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/17 12:21:30 by ibouchla          #+#    #+#              #
#    Updated: 2016/04/14 22:29:55 by ibouchla         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NM_BIN			=	ft_nm
OTOOL_BIN		=	ft_otool

INC_PATH		=	-I includes/ -I libft/includes/
SRC_PATH		=	srcs

SRC_NAME		=	 file_functions.c init_flags.c check_header_file.c tmp.c

SRC				=	$(addprefix $(SRC_PATH)/,$(SRC_NAME))
OBJET			=	$(SRC:.c=.o)

CC_FLAGS		=	clang -Wall -Wextra -Werror -std=c11 -pedantic #-Wunreachable-code

NM_LINK_LIB		=	$(CC_FLAGS) -o $(NM_BIN) $(OBJET) srcs/ft_nm.c -I includes/ -I libft/includes/ -L libft -lft
OTOOL_LINK_LIB	=	$(CC_FLAGS) -o $(OTOOL_BIN) $(OBJET) srcs/ft_otool.c -I includes/ -I libft/includes/ -L libft -lft

RED				=	\033[1;31m
BLUE			=	\033[1;34m
GREEN			=	\033[0;32m
YELLOW			=	\033[1;33m

all: $(NM_BIN) $(OTOOL_BIN)

$(NM_BIN): $(OBJET)
	@echo "➜    $(BLUE)Compilation of object files is complete.\n"
	@echo "➜    $(YELLOW)Compilation of additional libraries in progress.."
	@make -C libft/ > /dev/null
	@echo "➜    $(BLUE)Compilation of additional libraries is complete.\n"
	@echo "➜    $(YELLOW)Creation of the library in progress.."
	@$(NM_LINK_LIB)
	@echo "➜    $(BLUE)Creation of the library is complete.\n"
	@echo "\033[1;32m➜    Done"

$(OTOOL_BIN): $(OBJET)
	@echo "➜    $(BLUE)Compilation of object files is complete.\n"
	@echo "➜    $(YELLOW)Compilation of additional libraries in progress.."
	@make -C libft/ > /dev/null
	@echo "➜    $(BLUE)Compilation of additional libraries is complete.\n"
	@echo "➜    $(YELLOW)Creation of the library in progress.."
	@$(OTOOL_LINK_LIB)
	@echo "➜    $(BLUE)Creation of the library is complete.\n"
	@echo "\033[1;32m➜    Done"

%.o: %.c
	@$(CC_FLAGS) -c $< $(INC_PATH) -o $@
	@echo "\033[1;32m➜    Compilation of object file from the library source file : $(GREEN)Success"
clean:
	@echo "\n$(RED)➜    Deleting object files of the library"
	@$(RM) $(OBJET)
	@echo "$(RED)➜    Deleting additional libraries"
	@make fclean -C libft/ > /dev/null

fclean: clean
	@echo "➜    $(RED)Remove the library\n"
	@$(RM) $(NM_BIN) $(OTOOL_BIN)

re: fclean all
