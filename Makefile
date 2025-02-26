CC = g++
CFLAGS = -Wall -std=c++11 -Iinclude
LDFLAGS = -lglfw -lGL

# Source files
SOURCES = src/cpu.cpp src/glad.c src/display.cpp
EXECUTABLE = chip8

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE)

.PHONY: all clean