CC = g++
CFLAGS = -c
DEBUG_FLAGS = -g -Wall -DDEBUG

TARGET = ichanged
OBJ = ichanged.o logger.o monitor.o watcher.o node.o watch.o file.o handler.o \
	window.o options.o event.o record.o error.o config.o

.PHONY: debug all clean

debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

all: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) -lncurses -lpthread -lconfig++
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
window.o: window.cpp window.h
	$(CC) $(CFLAGS) window.cpp
options.o: options.cpp options.h
	$(CC) $(CFLAGS) options.cpp
event.o: event.cpp event.h
	$(CC) $(CFLAGS) event.cpp
record.o: record.cpp record.h
	$(CC) $(CFLAGS) record.cpp
error.o: error.cpp error.h
	$(CC) $(CFLAGS) error.cpp
config.o: config.cpp config.h
	$(CC) $(CFLAGS) config.cpp

clean:
	-rm $(OBJ) $(TARGET)
