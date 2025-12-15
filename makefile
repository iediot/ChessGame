CXX := g++
SRCS ?: main.cpp piece_moves.cpp
OBJS := $(SRCS:.cpp=.o)
TARGET := chess

SDL_CFLAGS := $(shell pkg-config --cflags sdl2 2>/dev/null)
SDL_LDFLAGS := $(shell pkg-config --libs sdl2 2>/dev/null)
EXTRA_LIBS := -lSDL2_image -lSDL2_gfx

CXXFLAGS ?= -std=c++17 -O2 $(SDL_CFLAGS)
LDFLAGS   ?= $(SDL_LDFLAGS) $(EXTRA_LIBS)

.PHONY: all debug clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

debug: CXXFLAGS := -std=c++17 -g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer $(SDL_CFLAGS)
debug: LDFLAGS := $(SDL_LDFLAGS) $(EXTRA_LIBS) -fsanitize=address,undefined
debug: clean $(TARGET)
	mv $(TARGET) $(TARGET)_debug || true
	@echo "Built debug binary: $(TARGET)_debug (note: sanitizer flags applied to linker too)"

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(OBJS) $(TARGET) $(TARGET)_debug

rebuild: clean all
