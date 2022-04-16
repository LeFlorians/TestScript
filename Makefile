CC := gcc
CFLAGS := -Wall -g

TARGET_EXEC := lang

.PHONY: all clean

SRCS := $(wildcard ./src/*.c)
OBJS := $(SRCS:%.c=%.o)

all: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET_EXEC)

test: all
	./$(TARGET_EXEC)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@


clean:
	rm $(TARGET_EXEC) $(wildcard ./src/*.o)