# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: csakamot <csakamot@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/01 14:21:20 by csakamot          #+#    #+#              #
#    Updated: 2024/11/17 01:27:28 by csakamot         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL = bash

##----Executable File Name-----##
NAME		= webserv
##-----------------------------##

##-----Directory Location------##
SRCDIR	= ./src/

INCDIR	= ./include/

SRCINC	= $(addprefix -I, $(shell find $(SRCDIR) -type d))

OBJDIR	= objs/
##-----------------------------##

##------------Srcs-------------##
CORESRC	=	main.cpp socketMain.cpp Socket.cpp

CONFSRC = configMain.cpp Config.cpp checkConfig.cpp isValidConfig.cpp isValidBracketFormat.cpp isValidDirectiveArguments.cpp isValidNest.cpp parseConfig.cpp

EVENSRC = eventLoop.cpp Epoll.cpp

HTTPSRC = httpServer.cpp Http.cpp HttpRequest.cpp AHttpMethod.cpp HttpDelete.cpp HttpGet.cpp HttpPost.cpp HttpResponse.cpp HttpErrorPage.cpp

ERROSRC = Error.cpp

UTILSRC = access.cpp charaOutput.cpp memory.cpp string.cpp net.cpp file.cpp time.cpp

SRCS		= $(addprefix $(SRCDIR)core/, ${CORESRC}) $(addprefix $(SRCDIR)config/, $(CONFSRC)) \
					$(addprefix $(SRCDIR)event/, $(EVENSRC)) $(addprefix $(SRCDIR)http/, $(HTTPSRC)) \
					$(addprefix $(SRCDIR)utils/, $(UTILSRC)) $(addprefix $(SRCDIR)error/, $(ERROSRC))
##-----------------------------##

##-----------Object------------##
OBJS		= ${subst ${SRCDIR}, $(OBJDIR), ${SRCS:.cpp=.o}}
##-----------------------------##

##----------conpiler-----------##
CC			= g++
CFLAGS	= -Wall -Wextra -Werror -std=c++98 -g -O0 -fsanitize=address
##-----------------------------##

##-----------archive-----------##
ARFLAG	= rcs
##-----------------------------##

##-------escape sequence-------##
RED			= \e[38;5;1m
GREEN		= \e[38;5;2m
ORANGE	= \e[38;5;3m
BLUE		= \e[38;5;4m
VIOLET	= \e[38;5;5m
AQUA		= \e[38;5;6m
WHITE		= \e[38;5;7m
GREY		= \e[38;5;8m
SET1		= \e[1;38;5;51m
RESET		= \e[0m
##-----------------------------##

##--------set variable---------##
COUNT		= 0
FILES		= 0
RATE		= 0.0
BAR			= 70
##-----------------------------##

##--------Makefile rule--------##
all:prev ${NAME}

prev:
				@$(eval FILES = $(shell find . -name "*.cpp" | wc -l))

${NAME}:${OBJS}
				@printf	"\n"
				@printf "$(SET1)                                        ,e, 888              888 888 888 $(RESET)\n"
				@printf "$(SET1) e88\'888  e88 88e  888 888 8e  888 88e   \"  888  ,e e,   e88 888 888 888 $(RESET)\n"
				@printf "$(SET1)d888  \'8 d888 888b 888 888 88b 888 888b 888 888 d88 88b d888 888 \"8\" \"8\" $(RESET)\n"
				@printf "$(SET1)Y888   , Y888 888P 888 888 888 888 888P 888 888 888   , Y888 888  e   e  $(RESET)\n"
				@printf "$(SET1) \"88,e8\'  \"88 88\"  888 888 888 888 88\"  888 888  \"YeeP\"  \"88 888 \"8\" \"8\" $(RESET)\n"
				@printf "$(SET1)                               888                                       $(RESET)\n"
				@printf "$(SET1)                               888                                       $(RESET)\n"
				@$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)%.o:$(SRCDIR)%.cpp
				@printf "\r\e[0m"
				@$(eval COUNT = $(shell echo $$(($(COUNT) + 1))))
				@printf "["
				@$(eval RATE = $(shell echo "scale=2; $(COUNT) / $(FILES) * $(BAR)" | bc | awk '{print int($$1)}'))
				@for i in {1..$(BAR)}; \
					do if [ $$i -le $(RATE) ]; \
						then printf "$(AQUA)=\e$(RESET)"; \
					else printf " "; \
					fi; \
				done
				@printf "]"
				@mkdir -p $(OBJDIR)core $(OBJDIR)config $(OBJDIR)event $(OBJDIR)http $(OBJDIR)error $(OBJDIR)utils
				@$(CC) $(CFLAGS) -I $(INCDIR) -I $(SRCINC) -c $< -o $@

clean:
				@${RM} -rf ${OBJDIR}
				@printf "\e[38;5;197mobject file removed\e[0m\n"

fclean:clean
				@${RM} -rf ${NAME}
				@printf "\e[38;5;197mexecutable file removed\e[0m\n"

re:fclean all

exe:re
				@make -s clean

.PHONY:all clean fclean re exe
##-----------------------------##
