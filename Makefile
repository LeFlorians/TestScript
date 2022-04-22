CC := gcc
CFLAGS := -Wall -g

TARGET := lang

.PHONY: all clean

SRCS := $(shell find src -type f -iname '*.c')
OBJS := $(SRCS:%.c=%.o)

all: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

test: all
	./$(TARGET)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm $(TARGET) $(shell find src -type f -iname '*.o')