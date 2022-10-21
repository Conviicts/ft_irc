NAME		:= ircserv
BOTNAME		:= bot

BUILD_DIR	:= .build
LIB_DIR		:= libs

# LIBS
LIBTCP_DIR	:= $(LIB_DIR)/libTcp
BONUS_DIR	:= ./bonus


SRCS		:= $(wildcard srcs/**/*.cpp) $(wildcard srcs/*.cpp)
BONUSSRCS	:= $(wildcard bonus/*.cpp)
CXX			:= c++
CXXFLAGS	:= -g -Wall -Wextra -Werror -Ofast -std=c++98 \
			   -I ./includes -I ./$(LIBTCP_DIR)/includes
LDFLAGS		:= -L ./$(LIBTCP_DIR) -ltcp
OBJ			:= $(SRCS:srcs/%.cpp=$(BUILD_DIR)/%.o)


$(NAME): $(OBJ)
	make -C $(LIBTCP_DIR)
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

all: $(NAME) $(CERT)
.PHONY: all

bonus: 
	@$(CXX) $(CXXFLAGS) -I $(BONUS_DIR)/includes $(BONUSSRCS) -o $(BOTNAME)
.PHONY: bonus

clean:
	make -C $(LIBTCP_DIR) clean
	@rm -rf $(BUILD_DIR)
.PHONY: clean

fclean: clean
	make -C $(LIBTCP_DIR) fclean
	@rm -rf $(NAME) $(BOTNAME) $(BIN_DIR)
.PHONY: fclean

re: fclean all
.PHONY: re

$(BUILD_DIR)/%.o: srcs/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(OBJ:.o=.d)
