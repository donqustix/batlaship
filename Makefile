CXX = g++
CXXFLAGS = -std=c++14 -pedantic -Wall -Wextra
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

LIB_NAME = BattleshipEngine
LIB_SRCS = $(wildcard src/engine/*.cpp)
LIB_OBJS = $(addprefix bin/, $(notdir $(LIB_SRCS:.cpp=.o)))

PROJECT_NAME = Battleship
PROJECT_SRCS = $(wildcard src/game/*.cpp)

all: $(LIB_NAME) $(PROJECT_NAME)

$(LIB_NAME): $(LIB_OBJS)
	ar rvs bin/$@.a $^

$(PROJECT_NAME): $(PROJECT_SRCS)
	$(CXX) $(CXXFLAGS) -Isrc $^ -o bin/$@ bin/$(LIB_NAME).a $(LDFLAGS)

run:
	./bin/$(PROJECT_NAME)

clean:
	rm -f $(LIB_OBJS) *.o

bin/%.o: src/engine/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

