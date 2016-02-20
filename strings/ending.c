#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main()
{
  int count = 10;

  int timeout = 3;
  printf("Capturing %d frame%s. Timeout: %d second%s\n", count, count == 1 ? "" : "s", timeout, timeout == 1 ? "" : "s");

  timeout = 1;
  printf("Capturing %d frame%s. Timeout: %d second%s\n", count, count == 1 ? "" : "s", timeout, timeout == 1 ? "" : "s");


  int boEqual = false;
  int id      = 100;
  int min     = 15;
  int max     = 20;

  if (min == max)
  {
    boEqual = true;
  } 

  printf("Illegal ID: %d. Id: %d%s", id, min, boEqual ? "" : "-");
  if (!boEqual)
  {
    printf("%d.\n", max);
  }

  max = 15;
  if (min == max)
  {
    boEqual = true;
  } 

  printf("Illegal ID: %d. Id: %d%s", id, min, boEqual ? "" : "-");
  if (!boEqual)
  {
    printf("%d.\n", max);
  }
  else
  {
    printf(".\n");
  }
  
  return 0;
}

