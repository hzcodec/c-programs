CC = gcc -std=c99
#CFLAGS = -c -Wall -Werror
CFLAGS = -c -Wall
LDFLAGS =

SRC_DIR = ../src
OBJ_DIR = ../obj

SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/file1.c $(SRC_DIR)/file2.c

OBJECTS = $(SOURCES:.c=.o)
$(info OBJECTS="$(OBJECTS)")
$(info -------------------------------------------------------------)
#$(info SOURCES="$(SOURCES)")

EXECUTABLE=main


all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	@echo "==========================="
	@echo "*** Create executable"
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

install: main
	mkdir -p ../release/bin
	mv $< ../release/bin/main

clean:
	rm -f main $(OBJECTS) ../release/bin/main

help:
	@echo "\033[91m To build type:\033[0m"
	@echo "\033[96m   make\033[0m"
	@echo "Files that are used:"
	@echo $(SOURCES)

