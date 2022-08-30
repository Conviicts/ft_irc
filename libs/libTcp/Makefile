NAME		:= libtcp.a

BUILD_DIR	:= .build

SRCS		:= $(wildcard srcs/*.cpp)
CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -Wpedantic -Wvla -Ofast -std=c++98 -MMD -MP -I ./includes

OBJS		:= $(SRCS:srcs/%.cpp=$(BUILD_DIR)/%.o)

$(NAME): $(OBJS)
	ar -rcs $@ $^

all: $(NAME)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

$(BUILD_DIR):
	mkdir $@

$(BUILD_DIR)/%.o: srcs/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(OBJS:.o=.d)

.PHONY: all clean fclean re