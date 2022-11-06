CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors -g -fsanitize=address
LIBS = -lm
OBJS = src/main.o src/ride.o src/user.o src/driver.o
TARGET = main

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

run: $(TARGET)
	./main

clean:
	@rm -f $(TARGET) $(OBJS)