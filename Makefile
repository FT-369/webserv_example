NAME			= webserv
CXX				= c++
CXXFLAGS		= -Wall -Wextra -Werror -std=c++98
INCLUDE			= -I./includes/

RM				= rm -rf
OBJDIR			= ./obj/
SRCDIR			= ./src/
SRCS			= main.cpp
					# $(SRC)/filename.cpp

OBJS_FILE		= $(SRCS:$(SRCDIR).cpp=.o)
OBJS			= $(addprefix $(OBJDIR), $(OBJS_FILE))

all:			objd_build	$(NAME)

$(NAME):		$(OBJS)
				$(CXX) $(CXXFLAGS) $(INCLUDE) $(OBJS) -o $(NAME)
				@echo "\033[0;92m* $(NAME) was created *\033[0m"

$(OBJDIR)%.o : %.cpp
				$(CXX) $(CXXFLAGS) $(INCLUDE) $< -c -o $@

objd_build		:
				mkdir -p $(OBJDIR)

clean:
				@$(RM) $(OBJDIR)
				@$(RM) $(OBJS)

fclean:			clean
				@$(RM) $(NAME)
				@echo "\033[0;91m* $(NAME) was removed *\033[0m"

re:				fclean all

.PHONY:			all clean fclean re bonus