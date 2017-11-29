#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesystem.h"

// RUN formatfs before conducting this test!


int main(int argc, char *argv[]) {
	int bitarray[128];

	bitarray[0] = 4294967295;
	bitarray[1] = 3;
	printf("block index = %d\n", get_empty_block_index(bitarray));
	set_bit(bitarray, get_empty_block_index(bitarray));
	printf("bit set\n");
	printf("new empty bit = %d\n", get_empty_block_index(bitarray));
	clear_bit(bitarray, get_empty_block_index(bitarray)-1);
	printf("bit cleared\n");
	printf("new empty bit = %d\n", get_empty_block_index(bitarray));
}