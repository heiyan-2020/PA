#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
	size_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_SERIAL, FD_EVENT,FD_INFO,FD_FB};
size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t serial_write(const void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len); 
size_t fb_write(const void *buf, size_t offset, size_t len);
/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, 0,invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, 0, invalid_read, invalid_write},
  [FD_STDERR] = {"stderr", 0, 0, 0, invalid_read, invalid_write},
	[FD_SERIAL] = {"serial", 0, 0, 0, invalid_read, serial_write},
	[FD_EVENT] = {"/dev/events", 0, 0, 0, events_read, invalid_write},
	[FD_INFO] = {"/proc/dispinfo", 0, 0, 0, dispinfo_read, invalid_write},
	[FD_FB] = {"/dev/fb", 0, 0, 0, invalid_read, fb_write},
#include "files.h"
};

void init_fs() {
  // TODO: initialize the size of /dev/fb
	int w = io_read(AM_GPU_CONFIG).width;
	int h = io_read(AM_GPU_CONFIG).height;
	file_table[FD_FB].size = w * h * 4;
}

size_t ramdisk_read(void*, size_t, size_t);
size_t ramdisk_write(const void *buf, size_t offset, size_t len); 
int fs_open(const char* filepath, int flags, int mode) {
	//find certain filepath
	size_t limit = sizeof(file_table);
	size_t sizeOfFinfo = sizeof(Finfo);
	for (int i = 0; i < limit / sizeOfFinfo; i ++) {
		if (strcmp(file_table[i].name, filepath) == 0) {
				file_table[i].open_offset = 0;
				return i;
		}
	}
	assert(0);
}

size_t fs_read(int fd, void* buf, size_t len) {
	//haven't handle exception.
	assert(0 <= fd && fd <= sizeof(file_table) / sizeof(Finfo));
	Finfo* currentFile = &file_table[fd];		
	if (currentFile->read == NULL) {
		if (currentFile->open_offset + len < currentFile->size) {
			ramdisk_read(buf, currentFile->disk_offset + currentFile->open_offset, len);
			currentFile->open_offset += len;
			return len;
		} else {
			ramdisk_read(buf, currentFile->disk_offset + currentFile->open_offset, currentFile->size - currentFile->open_offset);
			size_t oldValue = currentFile->open_offset;
			currentFile->open_offset = currentFile->size;
			return currentFile->size - oldValue;
		}
	} else {
		size_t amount = currentFile->read(buf,currentFile->open_offset,len);
		currentFile->open_offset += amount;
		return amount;
	}
}

int fs_close(int fd) {
	return 0;
}

size_t fs_write(int fd, const void* buf, size_t len) {
		//haven't handle exception.
	assert(0 <= fd && fd <= sizeof(file_table) / sizeof(Finfo));
	Finfo* currentFile = &file_table[fd];		
	if (currentFile->write == NULL) {
		if (currentFile->open_offset + len < currentFile->size) {
			ramdisk_write(buf, currentFile->disk_offset + currentFile->open_offset, len);
			currentFile->open_offset += len;
			return len;
		} else {
			ramdisk_write(buf, currentFile->disk_offset + currentFile->open_offset, currentFile->size - currentFile->open_offset);
			currentFile->open_offset = currentFile->size;
			return currentFile->size - currentFile->open_offset;
		}
	} else {
		size_t amount = currentFile->write(buf,currentFile->open_offset,len);
		currentFile->open_offset += amount;
		return amount;
	}
}
size_t fs_lseek(int fd, size_t offset, int whence) {
	assert(0 <= fd && fd <= sizeof(file_table) / sizeof(Finfo));
	Finfo* currentFile = &file_table[fd];
	uint32_t old = currentFile->open_offset;
	switch (whence) {
		case SEEK_SET: {
											currentFile->open_offset = offset > currentFile->size ? currentFile->size : offset;
											return currentFile->open_offset;	
									 }
		case SEEK_CUR: {
											currentFile->open_offset = offset + currentFile->open_offset > currentFile->size ? currentFile->size : offset + currentFile->open_offset;
											return currentFile->open_offset - old;
									 
									 }
		case SEEK_END: {
											currentFile->open_offset = currentFile->size;
											return currentFile->open_offset;
									 }
		default: assert(0);
	}	 
}

