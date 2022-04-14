CC := gcc
CFLAGS := -Wall -g

TARGET_EXEC := lang

.PHONY: all clean

SRCS := $(wildcard ./src/*.c)
OBJS := $(SRCS:%.c=%.o)

test: all
	./$(TARGET_EXEC)

all: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET_EXEC)


%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@


clean:
	rm $(TARGET_EXEC) $(wildcard ./src/*.o)