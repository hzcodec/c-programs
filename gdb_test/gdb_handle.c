/*
    Auther       : Heinz Samuelsson
    Date         : 2016-08-12
    File         : gdb_handle.c
    Reference    : http://nirbhay.in/blog/2012/09/debug-signal-handlers-using-gdb/ 
    Description  : How to debug signal handlers using gdb.
          
	           Compile for normal run:
		     gcc gdb_handle.c -o handle

	           Compile for gdb:
		     gcc gdb_handle.c -g -o gdb_handle 

                   The signal that we are trying to send to the program 
		   is being intercepted by GDB and hence control never 
		   enters the signal handler 
		   (where we have already set a breakpoint).

                   1. Use gdb command handle
                        (gdb) handle SIGINT nostop pass
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>  // SIGINT
#include <unistd.h>  // sleep()

void signalHandler(int signum)
{
  printf("SIGINT cought: %d\n", signum);
}
 
 
int main()
{

  signal(SIGINT, (void*)signalHandler);

  while(1)
  {
    sleep(1);
  }

  return 0;
}
