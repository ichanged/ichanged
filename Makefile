CC = g++
CFLAGS = -c
DEBUG_FLAGS = -g -Wall -DDEBUG

TARGET = ichanged
OBJ = ichanged.o monitor.o logger.o

.PHONY: debug all clean

debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

all: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)
ichanged.o: ichanged.cpp
	$(CC) $(CFLAGS) ichanged.cpp
monitor.o: monitor.cpp monitor.h
	$(CC) $(CFLAGS) monitor.cpp
logger.o: logger.cpp logger.h
	$(CC) $(CFLAGS) logger.cpp

clean:
	-rm $(OBJ) $(TARGET)
