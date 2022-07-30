# specify destination files
TARGET := lang
TEST_INPUT = test.txt

# general compiler flags
CC := gcc
LIBS := -lm
CFLAGS := -Wall -pg -Og -g -Wno-switch -Wno-return-type

# apply optimizations for the release target
release: CFLAGS := -Wall -O3

# define recursive wildcard function
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# search for .c files in ./src
SRCS := $(call rwildcard,src/,*.c)
OBJS := $(SRCS:%.c=%.o)

.PHONY: all clean

all: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o $(TARGET)

release: all

test: all
	clear
	$(file >> $(TEST_INPUT))
	./$(TARGET) $(TEST_INPUT)

profile: all
	clear
	$(file >> $(TEST_INPUT))
	@echo Running test program for analysis...
	./$(TARGET) $(TEST_INPUT) > /dev/null
	@echo Analysis:
	gprof -p -b ./$(TARGET) gmon.out

memcheck: all
	valgrind --tool=memcheck ./$(TARGET) $(TEST_INPUT)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# target to generate mapop.c
# requires gperf to be installed
mapop:
	gperf --output-file=./src/mappings/mapop.c ./mapop.gperf

# set windows compiler and define target
# do not use this target on windows
win : CC = x86_64-w64-mingw32-gcc 
win: all

clean:
	rm -f -- $(TARGET) $(TARGET).exe gmon.out $(call rwildcard,src/,*.o)