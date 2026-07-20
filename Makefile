#includes "notes for myself because this shi complicated.h"

TARGET = bin\main.exe # compilation target

CC = gcc # compiler we use and flags for it
CFLAGS = -Wall -Wextra -Iinclude -Isrc

LDFLAGS = -Llib -lSDL3 # linker flags

SRCS = $(wildcard src/*.c) # var storing all c files. $() is used for functions and variable addressing i think

OBJS = $(patsubt src/%.c, build/%.o, $(SRCS)) # patsubt takes pattern, what to place, source. (% is a wildcard for strings)

DEPS = $(OBJS:.o=.d) 

.PHONY = all clean run # i think we need this

# i dont bother with stuff below..
all: $(TARGET)

$(TARGET): $(OBJS)
	@if not exist build mkdir build
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

run: all
	$(TARGET)

clean:
	@if exists build rmdir /s /q build
	@if exists bin rmdir /s /q bin