CC     = gcc
CFLAGS = -std=gnu99 -Wall -Wextra
TARGET = fsm
TARGETDIR = /usr/local/bin

#all: hsm_control.c
#	$(CC) -c statemachine.c
#	$(CC) -c queue.c
#	$(CC) -c hsm_main.c
#	$(CC) -c hsm_power.c
#	$(CC) -c hsm_control.c
#	$(CC) statemachine.o queue.o hsm_main.o hsm_power.o hsm_control.o -o $(TARGET)
all: $(TARGET)

#$(TARGET): statemachine.o queue.o hsm_main.o hsm_power.o hsm_control.o test_fsm.o
$(TARGET): statemachine.o queue.o hsm_control.o hsm_test.o
	$(LINK.o) -o $@ $^

install:
	cp fsm $(TARGETDIR)/$(TARGET)
	rm fsm

clean:
	rm -f *.o fsm
