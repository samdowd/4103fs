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

File lookup_file(char *name) {
  char block[SOFTWARE_DISK_BLOCK_SIZE];

  read_sd_block(block, 257);

  File file;
  printf("returning empty file from lookup: %s\n\n\n", block);
  return file;
}

// open existing file with pathname 'name' and access mode 'mode'.  Current file
// position is set at byte 0.  Returns NULL on error. Always sets 'fserror' global.
File open_file(char *name, FileMode mode) {
  lookup_file(name);
  printf("lookup done for %s\n\n\n", name);
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
  // find first zero in dir_bit_vector, let's say it was at 9
  file->dir_block_index = 9;
  dir_bit_vector[9] = 1;
  write_sd_block(dir_bit_vector, 257);

  // check inode bit vector for 0 - update it to 1
  int inode_bit_vector[SOFTWARE_DISK_BLOCK_SIZE];
  read_sd_block(inode_bit_vector, 258);
  // find first zero in inode bit vector, let's say 7
  file->inode_block_index = 7;
  inode_bit_vector[7] = 1;
  write_sd_block(inode_bit_vector, 258);  

	// go to empty dir block - store name and empty inode block index

	// call open_file()
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

// filesystem error code set (set by each filesystem function)
FSError fserror;

