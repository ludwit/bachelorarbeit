CC = gcc 
CFLAGS = -g -Wall -Wextra -pedantic -O3
LDFLAGS =  -lsodium

SOURCES = test
OBJECTS = $(SOURCES:.c=.o)
TARGET = test

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	@rm -f $(TARGET) $(OBJECTS) core
