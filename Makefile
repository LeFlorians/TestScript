CC := gcc
CFLAGS := -Wall -g

TEST_INPUT = test.txt

TARGET := lang

# Define recursive wildcard function
rwildcard=$(wildcard $1$2)$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

.PHONY: all clean

SRCS := $(call rwildcard,src/,*.c)
OBJS := $(SRCS:%.c=%.o)

all: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

test: all
	clear
	$(file >> $(TEST_INPUT))
	./$(TARGET) $(TEST_INPUT)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# target to generate mapop.c
# requires gperf to be installed
mapop:
	gperf --output-file=src/mapop.c mapop.gperf

# set windows compiler and define target
# do not use this target on windows
win : CC = x86_64-w64-mingw32-gcc 
win: all

clean:
	rm -f -- $(TARGET) $(TARGET).exe $(call rwildcard,src/,*.o)