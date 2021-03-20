CC=g++
CFLAGS=-c -Wall -lSDL
LDFLAGS=-lSDL -lSDL_image
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=AppBin

all: $(SOURCES) $(EXECUTABLE)
	rm $(OBJECTS)
	read
	clear
	./$(EXECUTABLE)
	read
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clear: