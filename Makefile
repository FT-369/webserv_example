NAME			= webserv
CXX				= c++
CXXFLAGS		= -Wall -Wextra -Werror -std=c++98

INCLUDE			= -I./includes/
RM				= rm -rf
OBJDIR			= ./obj/
SRCDIR			= ./src/
MAIN			= main.cpp
SRCS			= ConfigParser.cpp

MAIN_OBJS_FILE	= $(MAIN:.cpp=.o)
MAIN_OBJS		= $(addprefix $(OBJDIR), $(MAIN_OBJS_FILE))

SRCS_OBJS_FILE	= $(SRCS:.cpp=.o)
SRCS_OBJS		= $(addprefix $(OBJDIR), $(SRCS_OBJS_FILE))

all:			objd_build	$(NAME)

objd_build:
				mkdir -p $(OBJDIR)

$(OBJDIR)%.o : ./%.cpp
				$(CXX) $(CXXFLAGS) $(INCLUDE) $< -c -o $@

$(OBJDIR)%.o : $(SRCDIR)%.cpp
				$(CXX) $(CXXFLAGS) $(INCLUDE) $< -c -o $@

$(NAME):		$(SRCS_OBJS) $(MAIN_OBJS)
				$(CXX) $(CXXFLAGS) $(INCLUDE) $(MAIN_OBJS) $(SRCS_OBJS) -o $(NAME)
				@echo "\033[0;92m* $(NAME) was created *\033[0m"

clean:
				@$(RM) $(OBJDIR)
				@$(RM) $(OBJS)

fclean:			
				@$(MAKE) clean
				@$(RM) $(NAME)
				@echo "\033[0;91m* $(NAME) was removed *\033[0m"

re:				
				@$(MAKE) fclean
				@$(MAKE) all


.PHONY:			all clean fclean re bonus