# Compiler and flags
CC      = cc
CFLAGS  = -Wall -Wextra -O2

# Target executable name
TARGET  = tictactoe

# Source files
SRCS    = main.c tui.c board.c
OBJS    = $(SRCS:.c=.o)

# Default rule
all: $(TARGET)

# Link object files into an executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compile each .c file into a .o
%.o: %.c tui.h
	$(CC) $(CFLAGS) -c $< -o $@

# Cleanup
clean:
	rm -f $(OBJS) $(TARGET)

debug: CFLAGS += -g
debug: $(TARGET)
