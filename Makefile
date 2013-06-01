CC = g++
CFLAGS = -c
DEBUG_FLAGS = -g -Wall -DDEBUG

TARGET = ichanged
OBJ = ichanged.o logger.o monitor.o watcher.o node.o watch.o file.o handler.o \
	option.o

.PHONY: debug all clean

debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

all: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)
ichanged.o: ichanged.cpp
	$(CC) $(CFLAGS) ichanged.cpp
logger.o: logger.cpp logger.h
	$(CC) $(CFLAGS) logger.cpp
monitor.o: monitor.cpp monitor.h
	$(CC) $(CFLAGS) monitor.cpp
watcher.o: watcher.cpp watcher.h
	$(CC) $(CFLAGS) watcher.cpp
node.o: node.cpp node.h
	$(CC) $(CFLAGS) node.cpp
watch.o: watch.cpp watch.h
	$(CC) $(CFLAGS) watch.cpp
file.o: file.cpp file.h
	$(CC) $(CFLAGS) file.cpp
handler.o: handler.cpp handler.h
	$(CC) $(CFLAGS) handler.cpp
option.o: option.cpp option.h
	$(CC) $(CFLAGS) option.cpp

clean:
	-rm $(OBJ) $(TARGET)
