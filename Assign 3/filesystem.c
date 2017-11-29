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
	char buf[SOFTWARE_DISK_BLOCK_SIZE];

	char name_read_in[507];
	int current_index;
	for(current_index = 1; current_index <= 128; current_index++){
		read_sd_block(buf, current_index);

  	memcpy(name_read_in, buf, 507);

  	if(strcmp(name, name_read_in) == 0)
  		break;
	}

	char mode_read_in = buf[507];
	char inode_index_read_in[5];
  memcpy(inode_index_read_in, &buf[508], 4*sizeof(*buf));

  int inode_index;
	sscanf(inode_index_read_in, "%d", &inode_index);

	FileMode m = (mode_read_in == 'r') ? READ_ONLY : READ_WRITE;

	File file;
	file->mode = m;
	memcpy(file->name, name_read_in, 507);
	file->dir_block_index = current_index;
	file->inode_block_index = inode_index;

	char inode_buf[512];
	read_sd_block(inode_buf, file->inode_block_index);

	file->is_open = inode_buf[0];
	// memcpy(file->direct_pointers, &inode_buf[1], 40*sizeof(*inode_buf));
	// memcpy(file->indirect_pointer, &inode_buf[41], 1*sizeof(*inode_buf));
	// memcpy(file->double_indirect_pointer, &inode_buf[42], 1*sizeof(*inode_buf));
	return file;
}

// open existing file with pathname 'name' and access mode 'mode'.  Current file
// position is set at byte 0.  Returns NULL on error. Always sets 'fserror' global.
File open_file(char *name, FileMode mode) {
	File file = lookup_file(name);
	// check if file is closed - throw error

	if(file->is_open == 'o'){
		fserror = FS_FILE_OPEN;
		return NULL;
	}

	file->is_open = 'o';
	// char inode_buffer[512];
	// read_sd_block(inode_buffer, file->inode_block_index);
	// inode_buffer[0] = 'o';
	// write_sd_block(inode_buffer, file->inode_block_index);

	return file;
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
	sprintf(index,"%04d",file->inode_block_index);
	for(int j=0; j<4; j++){
		dir_buf[508+j] = index[j];
	}

	// go to empty dir block - store name and empty inode block index
	write_sd_block(dir_buf, file->dir_block_index);

	char inode_buf[512] = {'c'};
	write_sd_block(inode_buf, file->inode_block_index);

	open_file(name, mode);
}

// close 'file'.  Always sets 'fserror' global.
void close_file(File file){

	if(file->is_open == 'c'){
		fserror = FS_FILE_NOT_OPEN;
	}

	// change inode bit to closed
	char inode_buffer[512];
	read_sd_block(inode_buffer, file->inode_block_index);
	inode_buffer[0] = 'o';
	write_sd_block(inode_buffer, file->inode_block_index);
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

