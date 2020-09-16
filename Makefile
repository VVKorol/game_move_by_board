
CC=g++
LDFLAGS= -lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi
CFLAGS=-std=c++14 -c
OBJECTS=$(SOURCES:.cpp=.o)
SOURCES= meta.cpp baseobj.cpp pawn.cpp game.cpp ui.cpp
EXECUTABLE=test2


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS)  -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

