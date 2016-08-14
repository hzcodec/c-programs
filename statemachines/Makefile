CC     = gcc
CFLAGS = -std=c99

all: statemachine_case.c
	$(CC) $(CFLAGS) statemachine_case.c -o fsm_case
	$(CC) $(CFLAGS) statemachine_case2.c -o fsm_case2
	$(CC) $(CFLAGS) statemachine_func_ptr.c -o fsm_func
	$(CC) $(CFLAGS) statemachine_func_ptr2.c -o fsm_func2

clean:
	rm -f fsm_case fsm_case2 fsm_func fsm_func2
