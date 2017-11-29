#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "softwaredisk.h"

int main(int argc, char *argv[]) {

  init_software_disk();
  printf("Size of software disk in blocks: %lu\n", software_disk_size());
  sd_print_error();

}