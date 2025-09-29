# Nome eseguibile
NAME = build/bakas.exe
#/c/Users/user/Desktop/Triple_Bakas
# Cartelle
SRC = src
BUILD = build
INCLUDE = include

# Compilatore e flags
CXX = g++
CXXFLAGS = -I$(INCLUDE) -Wall -Wextra -Werror -std=c++17 -g -I/mingw64/include

# Linker flags (SFML)
LDFLAGS = -L/mingw64/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# File sorgenti e oggetti
SRCS := $(wildcard $(SRC)/*.cpp)
OBJS := $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(SRCS))

# Regola principale
all: $(NAME)

# Creazione eseguibile
$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Creazione file oggetto
$(BUILD)/%.o: $(SRC)/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crea cartella build se non esiste
$(BUILD):
	mkdir $(BUILD)

# Pulizia oggetti
clean:
	rm -f $(OBJS)

# Pulizia oggetti + eseguibile
fclean: clean
	rm -f $(NAME)

# Ricompila tutto
re: fclean all

.PHONY: all clean fclean re
