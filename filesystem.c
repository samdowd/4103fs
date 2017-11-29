//
// Filesystem implementation for LSU 4103
// Written by Ben Graham and Sam Dowd, 11/2017
//

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "filesystem.h"
#include "softwaredisk.h"

// file system error code set (set by each file system function).
FSError fserror;

// typedef struct DirBlock {
// 	char buf[504];
// 	int mode;
// 	int inode;
// } DirBlock;

File lookup_file(char *name) {
	char block[SOFTWARE_DISK_BLOCK_SIZE];

	read_sd_block(block, 257);

	File file;
	// printf("returning empty file from lookup: %s\n\n\n", block);
	return file;
}

// open existing file with pathname 'name' and access mode 'mode'.  Current file
// position is set at byte 0.  Returns NULL on error. Always sets 'fserror' global.
File open_file(char *name, FileMode mode) {
	lookup_file(name);
	// printf("lookup done for %s\n\n\n", name);
	// check if file is closed - throw error
	// change inode bit to open
}

// create and open new file with pathname 'name' and access mode 'mode'.  Current file
// position is set at byte 0.  Returns NULL on error. Always sets 'fserror' global.
File create_file(char *name, FileMode mode){

	File file;

	// check dir bit vector for 0 - update it to 1
	int dir_bit_vector[SOFTWARE_DISK_BLOCK_SIZE];
	read_sd_block(dir_bit_vector, 257);

	int first_dir_zero = get_empty_block_index(dir_bit_vector);
	file->dir_block_index = first_dir_zero;
	set_bit(dir_bit_vector, first_dir_zero);

	write_sd_block(dir_bit_vector, 257);

	// check inode bit vector for 0 - update it to 1
	int inode_bit_vector[SOFTWARE_DISK_BLOCK_SIZE];
	read_sd_block(inode_bit_vector, 258);

	int first_inode_zero = get_empty_block_index(inode_bit_vector);
	file->inode_block_index = first_inode_zero;
	set_bit(inode_bit_vector, first_inode_zero);

	write_sd_block(inode_bit_vector, 258);

	// Write 512 byte buffer to the first empty directory block
	// dir_buf = name [507 bytes], mode [1 byte], block index [1 byte]
	char dir_buf[SOFTWARE_DISK_BLOCK_SIZE];

	// write name to buffer
	memcpy(dir_buf, name, sizeof(name)-1);
	for(int i = sizeof(name); i < 507; i++){
		dir_buf[i] = ' ';
	}

	// write mode
	dir_buf[507] = (mode == READ_ONLY) ? 'r' : 'w';

	// write index
	char index[5];
	sprintf(index,"%04d",file->dir_block_index);
	for(int j=0; j<4; j++){
		dir_buf[508+j] = index[j];
	}

	printf("Dir_buff:\n");

	//print name
	for (int j=0; j < 507; j++) {
    printf("%c", dir_buf[j]);
  }

  //print mode
  printf("%c", dir_buf[507]);

	//print index
	for (int j=0; j < 4; j++) {
    printf("%c", dir_buf[508+j]);
  }


	// go to empty dir block - store name and empty inode block index
	write_sd_block(dir_buf, file->dir_block_index);


	char buf[SOFTWARE_DISK_BLOCK_SIZE];
  read_sd_block(buf, file->dir_block_index);

  printf("\n++++++++++++++++ read_sd_block response +++++++++++++++++\n");

  for (int j=0; j < SOFTWARE_DISK_BLOCK_SIZE; j++) {
    printf("%c", buf[j]);
  }

  char name_read_in[507];
  for(int j=0; j < 507; j++){
  	name_read_in[j] = buf[j];
  	// printf("ours='%c' buf='%c'\n", name_read_in[j], buf[j]);
  }

  // memcpy(name_read_in, buf, 507);

  char mode_read_in = buf[507];
  
  char index_read_in[5];
  memcpy(index_read_in, &buf[508], 4*sizeof(*buf));




  for(int x=0; x < 4; x++){
  	index_read_in[x] = buf[508+x];
  }

  printf("\n++++++++++++++++ funky town +++++++++++++++++\n");
  for (int j=0; j < 507; j++) {
    printf("%c", name_read_in[j]);
  }

  printf("\n++++++++++++++++ Read to Variables +++++++++++++++++\n");


  int index_as_int;
	sscanf(index_read_in, "%d", &index_as_int);

  //print name
  for (int j=0; j < 507; j++) {
    printf("%c", name_read_in[j]);
  }

  //print mode
  printf("%c", mode_read_in);
  
  //print index
  printf("%d\n", index_as_int);

	open_file(name, mode);
}

// close 'file'.  Always sets 'fserror' global.
void close_file(File file){
	// check if file is open - throw error
	// release memory for FileInternal?..
	// change inode bit to closed
}

// read at most 'numbytes' of data from 'file' into 'buf', starting at the 
// current file position.  Returns the number of bytes read. If end of file is reached,
// then a return value less than 'numbytes' signals this condition. Always sets
// 'fserror' global.
unsigned long read_file(File file, void *buf, unsigned long numbytes){

}

// write 'numbytes' of data from 'buf' into 'file' at the current file position. 
// Returns the number of bytes written. On an out of space error, the return value may be
// less than 'numbytes'.  Always sets 'fserror' global.
unsigned long write_file(File file, void *buf, unsigned long numbytes){

}

// sets current position in file to 'bytepos', always relative to the beginning of file.
// Seeks past the current end of file should extend the file. Always sets 'fserror'
// global.
void seek_file(File file, unsigned long bytepos){

}

// returns the current length of the file in bytes. Always sets 'fserror' global.
unsigned long file_length(File file){

}

// deletes the file named 'name', if it exists and if the file is closed. 
// Fails if the file is currently open. Returns 1 on success, 0 on failure. 
// Always sets 'fserror' global.   
int delete_file(char *name){
	// check dir for name - get block number
	File file = lookup_file(name);

	// grab and reset dir bit vector for block number to 0
	// dir_bit_vector[file->dir_block_index] = 0;

	// reset inode bit vector for block grabbed to 0
	// like above but for inode
} 

// determines if a file with 'name' exists and returns 1 if it exists, otherwise 0.
// Always sets 'fserror' global.
int file_exists(char *name){

}

// describe current filesystem error code by printing a descriptive message to standard
// error.
void fs_print_error(void){

}

// Set the bit at the k-th position in bitarray[i]
void  set_bit(int bitarray[],  int k){
	bitarray[k/32] |= 1 << (k%32);  
}

// Clear the bit at the k-th position in bitarray[i]
void  clear_bit(int bitarray[],  int k){
	bitarray[k/32] &= ~(1 << (k%32));
}

int get_empty_block_index(int bitarray[]) {
	unsigned int i = 0;
	while(i < 128 && 0xFFFFFFFF == bitarray[i]){
		i++;
	}
	int int_with_zero = (int)(~((unsigned int)bitarray[i]));

	if(ffs(int_with_zero) == 0)
		return -1;
	else
		return i ? ffs(int_with_zero) + i*32 - 1 : ffs(int_with_zero) + i*32;
}

// filesystem error code set (set by each filesystem function)
FSError fserror;

