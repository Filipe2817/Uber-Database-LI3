CC = gcc
CFLAGS = -Wall -Wextra -Wwrite-strings -Wno-parentheses -Wpedantic -Warray-bounds -pedantic-errors -g -fsanitize=address 
LIBS = -lm
OBJS = src/main.o src/ride.o src/user.o src/driver.o src/parser.o src/utils.o src/date.o
TARGET = main
PKG_CONFIG = `pkg-config --cflags --libs glib-2.0`

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

run: $(TARGET)
	./main

clean:
	@rm -f $(TARGET) $(OBJS)