

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include "mymodule.h"


#define FILEPATH "/dev/mymodule0"

#define TEST_SIZE 10

int main(void)
{
  int fn = -1;
  char buf[TEST_SIZE];
  printf("Attempting to open file module file...\n");
  fn = open(FILEPATH, O_RDWR);
  if (fn < 0)
  {
    printf("Failed to open file!\n");
    return -1;
  }
  printf("Attempting to read from the file...\n");
  read(fn, &buf, TEST_SIZE);
  printf("Finished reading from file\n");
  return 0;
}
