// access mode for open_file() and create_file() 
typedef enum {
  READ_ONLY, READ_WRITE
} FileMode;

// main private file type
typedef struct FileInternals {
		FileMode mode;
    char name[507];
    int dir_block_index;
    int inode_block_index;
    int direct_pointers[10];
    int indirect_pointer;
    int double_indirect_pointer;
    int is_open;
    // position buffer?
} FileInternals;

// file type used by user code
typedef FileInternals* File;

// error codes set in global 'fserror' by filesystem functions
typedef enum  {
  FS_NONE, 
  FS_OUT_OF_SPACE,        // the operation caused the software disk to fill up
  FS_FILE_NOT_OPEN,  	  // attempted read/write/close/etc. on file that isn’t open
  FS_FILE_OPEN,      	  // file is already open. Concurrent opens are not
                          // supported and neither is deleting a file that is open.
  FS_FILE_NOT_FOUND, 	  // attempted open or delete of file that doesn’t exist
  FS_FILE_READ_ONLY, 	  // attempted write to file opened for READ_ONLY
  FS_FILE_ALREADY_EXISTS  // attempted creation of file with existing name
} FSError;

// function prototypes for filesystem API

// open existing file with pathname 'name' and access mode 'mode'.  Current file
// position is set at byte 0.  Returns NULL on error. Always sets 'fserror' global.
File open_file(char *name, FileMode mode);

// create and open new file with pathname 'name' and access mode 'mode'.  Current file
// position is set at byte 0.  Returns NULL on error. Always sets 'fserror' global.
File create_file(char *name, FileMode mode);

// close 'file'.  Always sets 'fserror' global.
void close_file(File file);

// read at most 'numbytes' of data from 'file' into 'buf', starting at the 
// current file position.  Returns the number of bytes read. If end of file is reached,
// then a return value less than 'numbytes' signals this condition. Always sets
// 'fserror' global.
unsigned long read_file(File file, void *buf, unsigned long numbytes);

// write 'numbytes' of data from 'buf' into 'file' at the current file position. 
// Returns the number of bytes written. On an out of space error, the return value may be
// less than 'numbytes'.  Always sets 'fserror' global.
unsigned long write_file(File file, void *buf, unsigned long numbytes);

// sets current position in file to 'bytepos', always relative to the beginning of file.
// Seeks past the current end of file should extend the file. Always sets 'fserror'
// global.
void seek_file(File file, unsigned long bytepos);

// returns the current length of the file in bytes. Always sets 'fserror' global.
unsigned long file_length(File file);

// deletes the file named 'name', if it exists and if the file is closed. 
// Fails if the file is currently open. Returns 1 on success, 0 on failure. 
// Always sets 'fserror' global.   
int delete_file(char *name); 

// determines if a file with 'name' exists and returns 1 if it exists, otherwise 0.
// Always sets 'fserror' global.
int file_exists(char *name);

// describe current filesystem error code by printing a descriptive message to standard
// error.
void fs_print_error(void);

void  set_bit(int bitarray[],  int k);

void  clear_bit(int bitarray[],  int k);

int get_empty_block_index(int bitarray[]);

// filesystem error code set (set by each filesystem function)
extern FSError fserror;
