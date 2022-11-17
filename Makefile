CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors -g -fsanitize=address `pkg-config --cflags --libs glib-2.0`
LIBS = -lm
OBJS = src/main.o src/ride.o src/user.o src/driver.o src/q1.o src/utils.o src/date.o
TARGET = main

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

run: $(TARGET)
	./main

clean:
	@rm -f $(TARGET) $(OBJS)