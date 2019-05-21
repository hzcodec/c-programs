CC = gcc -std=c99
#CFLAGS = -c -Wall -Werror
CFLAGS = -c -Wall
LDFLAGS =

SRC_DIR = ../src
OBJ_DIR = ../obj

SOURCES = $/main.c \
          $/f1.c \
          $/f2.c \
	  $/f3.c

OBJECTS = $(SOURCES:.c=.o)
$(info OBJECTS="$(OBJECTS)")
$(info -------------------------------------------------------------)
#$(info SOURCES="$(SOURCES)")


EXECUTABLE=main

c_files := $(wildcard *.c)
$(info c_files="$(c_files)")
$(info -------------------------------------------------------------)


all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	@echo "==========================="
	@echo "*** Create executable"
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

install: main
	mkdir -p release
	mv $< release/main

.PHONY: clean
clean:
	rm -f main $(OBJECTS) /release/main

help:
	@echo "\033[91m To build type:\033[0m"
	@echo "\033[96m   make\033[0m"
	@echo "Files that are used:"
	@echo $(SOURCES)

