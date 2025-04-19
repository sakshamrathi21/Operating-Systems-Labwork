#include "simplefs-ops.h"
extern struct filehandle_t file_handle_array[MAX_OPEN_FILES]; // Array for storing opened files

int simplefs_create(char *filename){
    /*
	    Create file with name `filename` from disk
	*/
	struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));
	// Check if filename already exists
	for(int i = 0; i < NUM_INODES; i++){
		simplefs_readInode(i, inode);
		if(inode->status == INODE_IN_USE && strcmp(inode->name, filename) == 0){
			free(inode);
			return -1;
		}
	}

	int inodeIndex = simplefs_allocInode();
	// Check if free inode block present
	if(inodeIndex == -1){
		free(inode);
		return -1;
	}

	strcpy(inode->name, filename);
	inode->status = INODE_IN_USE;
	inode->file_size = 0;
	for(int i = 0; i < MAX_FILE_SIZE; i++){
		inode->direct_blocks[i] = -1;
	}
	simplefs_writeInode(inodeIndex, inode);
	free(inode);
    return inodeIndex;
}


void simplefs_delete(char *filename){
    /*
	    delete file with name `filename` from disk
	*/
	struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));
	for(int i = 0; i < NUM_INODES; i++){
		simplefs_readInode(i, inode);
		if(inode->status == INODE_IN_USE && strcmp(inode->name, filename) == 0){
			for(int j = 0; j < MAX_FILE_SIZE; j++){
				if(inode->direct_blocks[j] != -1){
					simplefs_freeDataBlock(inode->direct_blocks[j]);
				}
			}
			simplefs_freeInode(i);
			free(inode);
			return;
		}
	}
	free(inode);
	return;
}

int simplefs_open(char *filename){
    /*
	    open file with name `filename`
	*/
	struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));
	// Check if filename is present
	int flag1 = -1;
	for(int i = 0; i < NUM_INODES; i++){
		simplefs_readInode(i, inode);
		if(inode->status == INODE_IN_USE && strcmp(inode->name, filename) == 0){
			flag1 = i;
			break;
		}
	}

	free(inode);
	// Check for available entry in file_handle_array
	int flag2 = -1;
	for(int i = 0; i < MAX_OPEN_FILES; i++){
		if(file_handle_array[i].inode_number == -1){
			flag2 = i;
			break;
		}
	}

	if(flag1 == -1 || flag2 == -1){
		return -1;
	}

	file_handle_array[flag2].inode_number = flag1;
	file_handle_array[flag2].offset = 0;
	return flag2;
}

void simplefs_close(int file_handle){
    /*
	    close file pointed by `file_handle`
	*/
	file_handle_array[file_handle].inode_number = -1;
	file_handle_array[file_handle].offset = 0;
}

int simplefs_read(int file_handle, char *buf, int nbytes){
    /*
	    read `nbytes` of data into `buf` from file pointed by `file_handle` starting at current offset
	*/
	if(nbytes <= 0) return -1;
	struct filehandle_t *fh = file_handle_array + file_handle;
	if(fh->inode_number < 0){
		return -1;
	}
	struct inode_t* inode = (struct inode_t*) malloc(sizeof(struct inode_t));
	simplefs_readInode(fh->inode_number, inode);
	if(inode->status == INODE_FREE){
		free(inode);
		return -1;
	}
	if(fh->offset + nbytes > inode->file_size){
		free(inode);
		return -1;
	}

	int blk = fh->offset / BLOCKSIZE;
	int off_in_blk = fh->offset % BLOCKSIZE;
	int bytes_left_to_read = nbytes;

	while(bytes_left_to_read > 0){
		char blk_data[BLOCKSIZE];
		simplefs_readDataBlock(inode->direct_blocks[blk], blk_data);
		if(off_in_blk + bytes_left_to_read > BLOCKSIZE){
			memcpy(buf + (nbytes - bytes_left_to_read), blk_data + off_in_blk, BLOCKSIZE - off_in_blk);
			blk++;
			bytes_left_to_read -= (BLOCKSIZE - off_in_blk);
			off_in_blk = 0;
		}
		else{
			memcpy(buf + (nbytes - bytes_left_to_read), blk_data + off_in_blk, bytes_left_to_read);
			bytes_left_to_read = 0;
			break;
		}
	}
	free(inode);
    return 0;
}


int simplefs_write(int file_handle, char *buf, int nbytes){
    /*
	    write `nbytes` of data from `buf` to file pointed by `file_handle` starting at current offset
	*/
    if(nbytes == 0) return 0;
	struct filehandle_t *fh = file_handle_array + file_handle;
	if(fh->inode_number < 0){
		return -1;
	}
	struct inode_t* inode = (struct inode_t*) malloc(sizeof(struct inode_t));
	simplefs_readInode(fh->inode_number, inode);
	if(inode->status == INODE_FREE){
		free(inode);
		return -1;
	}
	if(nbytes + fh->offset > MAX_FILE_SIZE * BLOCKSIZE){
		free(inode);
		return -1;
	}
	int num_blks = 0;
	for (int i = 0; i < MAX_FILE_SIZE; i++)
	{
		if(inode->direct_blocks[i] != -1){
			num_blks++;
		}
	}
	
	if((fh->offset % BLOCKSIZE == 0) && (fh->offset / BLOCKSIZE < MAX_FILE_SIZE) && (fh->offset == inode->file_size) && (inode->direct_blocks[fh->offset / BLOCKSIZE] == -1)){
		/* 
			In the edge case offset is present at the end of last allocated datablock, 
			we request another datablock to make it work for general case handled after this if cond.
		*/
		inode->direct_blocks[num_blks] = simplefs_allocDataBlock();
		if(inode->direct_blocks[num_blks] == -1){
			return -1;
		}
		char nulldata[BLOCKSIZE];
		memset(nulldata, 0, BLOCKSIZE);
		simplefs_writeDataBlock(inode->direct_blocks[num_blks], nulldata);
		num_blks++;
	}

	int blk = fh->offset / BLOCKSIZE;
	int off_in_blk = fh->offset % BLOCKSIZE;
	int nbytes_avail = (num_blks - (blk + 1)) * BLOCKSIZE + (BLOCKSIZE - off_in_blk);
	int nbytes_req = (nbytes - nbytes_avail > 0) ? nbytes - nbytes_avail : 0;
	int nblocks_req = nbytes_req/BLOCKSIZE + (nbytes_req % BLOCKSIZE ? 1 : 0);
	int new_blocks[nblocks_req];
	for(int i = 0; i < nblocks_req; i++){
		new_blocks[i] = -1;
	}
	for(int i = 0; i < nblocks_req; i++){
		new_blocks[i] = simplefs_allocDataBlock();
		if(new_blocks[i] < 0){
			// not enough datablocks, free up newly allocated ones, and return
			for(int i = 0; i < nblocks_req; i++){
				if(new_blocks[i] >= 0){
					simplefs_freeDataBlock(new_blocks[i]);
				}
			}
			free(inode);
			return -1;
		}
	}

	for(int i = 0; i < nblocks_req; i++){
		char nulldata[BLOCKSIZE];
		memset(nulldata, 0, BLOCKSIZE);
		simplefs_writeDataBlock(new_blocks[i], nulldata);
	}

	assert(nblocks_req + num_blks <= MAX_FILE_SIZE);
	for(int i = num_blks; i < nblocks_req + num_blks; i++){
		inode->direct_blocks[i] = new_blocks[i - num_blks];
	}
	int bytes_left_to_write = nbytes;
	while((blk < MAX_FILE_SIZE) && (bytes_left_to_write > 0)){
		char blk_data[BLOCKSIZE];
		simplefs_readDataBlock(inode->direct_blocks[blk], blk_data);
		if(off_in_blk + bytes_left_to_write > BLOCKSIZE){
			memcpy(blk_data + off_in_blk, buf + (nbytes - bytes_left_to_write) , BLOCKSIZE - off_in_blk);
			simplefs_writeDataBlock(inode->direct_blocks[blk], blk_data);
			blk++;
			bytes_left_to_write -= (BLOCKSIZE - off_in_blk);
			off_in_blk = 0;
		}
		else{
			memcpy(blk_data + off_in_blk, buf + (nbytes - bytes_left_to_write), bytes_left_to_write);
			simplefs_writeDataBlock(inode->direct_blocks[blk], blk_data);
			bytes_left_to_write = 0;
			break;
		}
	}
	inode->file_size = (fh->offset + nbytes > inode->file_size) ? (fh->offset + nbytes) : inode->file_size;
	simplefs_writeInode(fh->inode_number, inode);
    return 0;
}


int simplefs_seek(int file_handle, int nseek){
    /*
	   increase `file_handle` offset by `nseek`
	*/
	if(file_handle_array[file_handle].inode_number == -1) return -1;

	int present_offset = file_handle_array[file_handle].offset;

	struct inode_t *inode = (struct inode_t *)malloc(sizeof(struct inode_t));
	simplefs_readInode(file_handle_array[file_handle].inode_number, inode);
	// Check if new_offset falls within the file's boundary
	if(present_offset < -nseek || present_offset + nseek > inode->file_size) {
		return -1;
	}
	file_handle_array[file_handle].offset += nseek;  
    return 0;
}