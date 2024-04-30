#include "simplefs-ops.h"
extern struct filehandle_t file_handle_array[MAX_OPEN_FILES]; // Array for storing opened files


int simplefs_create(char *filename){
    /*
	    Create file with name `filename` from disk
	*/
	for (int i = 0 ; i < NUM_INODES ; i ++ ) {
		struct inode_t check_ptr;
		simplefs_readInode(i, &check_ptr);
		if (check_ptr.status == INODE_FREE) continue;
		if (strcmp(check_ptr.name,filename) == 0) return -1;
	}
	int new_inode_num = simplefs_allocInode();
	if (new_inode_num == -1) return -1;
	struct inode_t check_ptr;
	simplefs_readInode(new_inode_num, &check_ptr);
	memcpy(check_ptr.name, filename, strlen(filename)+1);
	check_ptr.status = INODE_IN_USE;
	simplefs_writeInode(new_inode_num, &check_ptr);
    return new_inode_num;
}


void simplefs_delete(char *filename){
    /*
	    delete file with name `filename` from disk
	*/
	for (int i = 0 ; i < NUM_INODES ; i ++ ) {
		struct inode_t check_ptr;
		simplefs_readInode(i, &check_ptr);
		if (check_ptr.status == INODE_FREE) continue;
		if (strcmp(check_ptr.name,filename) == 0) {
			for (int j = 0 ; j < MAX_FILE_SIZE ; j ++ ) {
				if (check_ptr.direct_blocks[j] == -1) continue;
				simplefs_freeDataBlock(check_ptr.direct_blocks[j]);
			}
			simplefs_freeInode(i);
			return;
		}
	}
}

int simplefs_open(char *filename){
    /*
	    open file with name `filename`
	*/
	for (int i = 0 ; i < NUM_INODES ; i ++ ) {
		struct inode_t check_ptr;
		simplefs_readInode(i, &check_ptr);
		if (check_ptr.status == INODE_FREE) continue;
		if (strcmp(check_ptr.name,filename) == 0) {
			for (int j = 0 ; j < MAX_OPEN_FILES ; j ++ ) {
				if (file_handle_array[j].inode_number == -1) {
					file_handle_array[j].inode_number = i;
					file_handle_array[j].offset = 0;
					return j;
				}
			}
		}
	}
    return -1;
}

void simplefs_close(int file_handle){
    /*
	    close file pointed by `file_handle`
	*/
	if (file_handle >= MAX_OPEN_FILES || file_handle < 0) return;
	file_handle_array[file_handle].inode_number = -1;
	file_handle_array[file_handle].offset = 0;
}

int simplefs_read(int file_handle, char *buf, int nbytes){
    /*
	    read `nbytes` of data into `buf` from file pointed by `file_handle` starting at current offset
	*/
	if (file_handle >= MAX_OPEN_FILES || file_handle < 0) return -1;
	if (file_handle_array[file_handle].inode_number == -1) return -1;
	if (file_handle_array[file_handle].offset + nbytes > MAX_FILE_SIZE*BLOCKSIZE) return -1;
	int current_offset = file_handle_array[file_handle].offset;
	char total_string[BLOCKSIZE*MAX_FILE_SIZE+1] = "";
	struct inode_t check_ptr;
	simplefs_readInode(file_handle_array[file_handle].inode_number, &check_ptr);
	for (int i = 0 ; i < MAX_FILE_SIZE ; i ++ ){
		char buf_block[BLOCKSIZE+1] = "";
		if (check_ptr.direct_blocks[i] == -1) break;
		simplefs_readDataBlock(check_ptr.direct_blocks[i], buf_block);
		strcat(total_string, buf_block);
	}
	for (int i = current_offset ; i < current_offset + nbytes ; i ++ ){
		buf[i-current_offset] = total_string[i];
	}
    return 0;
}


int simplefs_write(int file_handle, char *buf, int nbytes){
    /*
	    write `nbytes` of data from `buf` to file pointed by `file_handle` starting at current offset
	*/
	if (file_handle >= MAX_OPEN_FILES || file_handle < 0) return -1;
	if (file_handle_array[file_handle].inode_number == -1) return -1;
	if (file_handle_array[file_handle].offset + nbytes > MAX_FILE_SIZE*BLOCKSIZE) return -1;
	int current_offset = file_handle_array[file_handle].offset;

	char total_string[BLOCKSIZE*MAX_FILE_SIZE+1] = "";
	struct inode_t check_ptr;
	simplefs_readInode(file_handle_array[file_handle].inode_number, &check_ptr);
	int read_bytes = 0;
	for (int i = 0 ; i < MAX_FILE_SIZE ; i ++ ) {
		if (read_bytes >= current_offset) break;
		char buf_block[BLOCKSIZE+1] = "";
		simplefs_readDataBlock(check_ptr.direct_blocks[i], buf_block);
		
		for (int j = 0 ; j < BLOCKSIZE ; j ++ ){
			total_string[read_bytes] = buf_block[j];
			read_bytes ++;
			if (read_bytes >= current_offset) break;
		}
	}
	strcat(total_string, buf);
	int bytes_written = 0;
	int total_blocks_available = 0;
	for (int i = 0 ; i < NUM_INODES; i ++ ) {
		struct inode_t check1_ptr;
		simplefs_readInode(i, &check1_ptr);
		for (int j = 0 ; j < MAX_FILE_SIZE ; j ++ ) {
			if (check1_ptr.direct_blocks[j] != -1) total_blocks_available ++;
		}
	}
	total_blocks_available = NUM_DATA_BLOCKS - total_blocks_available;
	for (int i = 0 ; i < MAX_FILE_SIZE ; i ++ ){
		if (check_ptr.direct_blocks[i] != -1) total_blocks_available ++;
	}
	int len_string = strlen(total_string);
	int required = 0;
	if (len_string%BLOCKSIZE == 0) required = len_string/BLOCKSIZE;
	else required = len_string/BLOCKSIZE + 1;
	if (required > total_blocks_available) return -1;
	for (int i = 0 ; i < MAX_FILE_SIZE ;  i ++ ){
		if (bytes_written >= strlen(total_string)) break;
		if (check_ptr.direct_blocks[i] == -1) {
			check_ptr.direct_blocks[i] = simplefs_allocDataBlock();
			if (check_ptr.direct_blocks[i] == -1) return -1;
		}
		char buf_block[BLOCKSIZE+1] = "";
		memcpy(buf_block, total_string + bytes_written, BLOCKSIZE);
		bytes_written += BLOCKSIZE;
		simplefs_writeDataBlock(check_ptr.direct_blocks[i], buf_block);
	}

	
	int count = 0;
	for (int i = 0 ; i < MAX_FILE_SIZE ;  i ++ ) {
		char buff[BLOCKSIZE+1] = "";
		if (check_ptr.direct_blocks[i] != -1) {
			simplefs_readDataBlock(check_ptr.direct_blocks[i], buff);
			
			count += strlen(buff);
			if (strlen(buff) == 0) {
				simplefs_freeDataBlock(check_ptr.direct_blocks[i]);
				check_ptr.direct_blocks[i] = -1;
			}
		}
	}
	check_ptr.file_size = count;
	simplefs_writeInode(file_handle_array[file_handle].inode_number, &check_ptr);
    return 0;
}


int simplefs_seek(int file_handle, int nseek){
    /*
	   increase `file_handle` offset by `nseek`
	*/
	if (file_handle >= MAX_OPEN_FILES || file_handle < 0) return -1;
	int value = file_handle_array[file_handle].offset + nseek;
	if (value < 0 || value > MAX_FILE_SIZE*BLOCKSIZE) return -1;
	file_handle_array[file_handle].offset = value;
    return 0;
}