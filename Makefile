CC := gcc
CFLAGS := -Wall -g

TARGET := lang

# Define recursive wildcard function
rwildcard=$(wildcard $1$2)$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

.PHONY: all clean

SRCS := $(call rwildcard,src/,*.c)
OBJS := $(SRCS:%.c=%.o)

all: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

test: all
	./$(TARGET)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# set windows compiler and define target
win : CC = x86_64-w64-mingw32-gcc 
win: all

clean:
	rm $(TARGET) $(call rwildcard,src/,*.o)