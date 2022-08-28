# specify destination files
TARGET := lang
TEST_INPUT = test.txt

# general compiler flags
CC := gcc
LIBS := -lm
CFLAGS := -Wall -pg -Og -g -Wno-switch -Wno-return-type

# apply optimizations for the release target
release: CFLAGS := -Wall -O3

# activate debug output for test release
test: CFLAGS += -DDEBUG

# define recursive wildcard function
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# search for .c files in ./src
SRCS := $(call rwildcard,src/,*.c)
OBJS := $(SRCS:%.c=%.o)

.PHONY: all clean

all: mapop $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o $(TARGET)

release: all

test: all
	clear
	$(file >> $(TEST_INPUT))
	./$(TARGET) $(TEST_INPUT)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

paper: paper.org
	pandoc -f org -t pdf paper.org -o paper.pdf

# target to generate mapop.c
# requires gperf to be installed
mapop: mapop.gperf
	gperf --output-file=./src//interpreter/mappings/mapop.c ./mapop.gperf

clean:
	rm -f -- $(TARGET) $(TARGET).exe gmon.out $(call rwildcard,src/,*.o)
