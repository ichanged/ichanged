CC = g++
CFLAGS = -c
DEBUG_FLAGS = -g -Wall -DDEBUG

TARGET = ichanged
OBJ = ichanged.o

.PHONY: debug all clean

debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

all: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)
ichanged.o: ichanged.cpp
	$(CC) $(CFLAGS) ichanged.cpp

clean:
	-rm $(OBJ) $(TARGET)
