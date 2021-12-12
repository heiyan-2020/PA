#include <proc.h>
#include <elf.h>
#include <fs.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
static Elf_Ehdr elf_header[1];
static Elf_Phdr prog_header[1];
size_t ramdisk_read(void*, size_t, size_t);
void load_page(PCB* pcb, int fd);

uintptr_t loader(PCB *pcb, const char *filename) {
	//read the elf_header.
	int fd = fs_open(filename, 0, 0);	
	assert(fd > 0);
	fs_read(fd, elf_header, sizeof(Elf_Ehdr));
	//verify file type.
	assert(*(uint32_t *)elf_header->e_ident == 0x464C457F);
	//read the program_header.
	size_t offset = elf_header->e_phoff;
	size_t len = elf_header->e_phentsize;
	for(uint32_t i = 0; i < elf_header->e_phnum; i++) {
		fs_lseek(fd, offset, SEEK_SET);
		fs_read(fd, prog_header, len);
		assert(prog_header->p_memsz >= prog_header->p_filesz);
		offset += len;
		//seg needs to be loaded.
		if (prog_header->p_type == 0x1) {
			load_page(pcb, fd);
			//uint8_t* vaddr = (uint8_t*)prog_header->p_vaddr;
			//fs_lseek(fd, prog_header->p_offset, SEEK_SET);
			//fs_read(fd, vaddr, prog_header->p_filesz);
		//	memset(vaddr + prog_header->p_filesz, 0, prog_header->p_memsz - prog_header->p_filesz);			
		}
	}
  return (uintptr_t)elf_header->e_entry;
}
static int pgsize;
void load_helper(void* buf, int fd, int current, int size) {
	if (current + size <= prog_header->p_filesz) {
		fs_read(fd, buf, size);
	} else if (current <= prog_header->p_filesz) {
		int rem = prog_header->p_filesz - current;
		assert(rem < size);
		fs_read(fd, buf, rem);
		memset(buf + rem, 0, size - rem);
//		uint32_t* pt = (uint32_t*) buf;
//		for (int i = 0; i < size; i+=4) {
//						printf("0x%x: 0x%x\n", prog_header->p_vaddr + current + i, *pt);
//			pt++;
//		}
	} else {
		memset(buf, 0, size);
	}
uint32_t* pt = (uint32_t*) buf;

for (int i = 0; i < size; i+=4) {
						printf("0x%x: 0x%x\n", prog_header->p_vaddr + current + i, *pt);
			pt++;
		}
}

void load_page(PCB* pcb, int fd) {
	pgsize = pcb->as.pgsize;
	int page_num = prog_header->p_memsz / pcb->as.pgsize + 1;
//	page_num = page_num > 0 ? page_num : 1;
	void* vaddr = (void*)prog_header->p_vaddr;
	//printf("vaddr = 0x%x, filesz = 0x%x, memsz = 0x%x\n", vaddr, prog_header->p_filesz, prog_header->p_memsz);
	AddrSpace* _as = &pcb->as;
	fs_lseek(fd, prog_header->p_offset, SEEK_SET);
	int current = 0;
	int offset = ((uint32_t)vaddr) & (pgsize - 1);
	if (offset != 0) {
		void* page_frame = new_page(1);
		map(_as, vaddr, page_frame, 1);
		load_helper(page_frame + offset, fd, current, pgsize - offset);
		vaddr += (pgsize - offset);
		current += pgsize - offset;
	}
//	uint8_t buf[prog_header->p_memsz];
//	fs_read(fd, buf, prog_header->p_filesz);
//	memset(buf + prog_header->p_filesz, 0, prog_header->p_memsz - prog_header->p_filesz);

	for (int i = 0; i < page_num; i++) {
		void* page_frame = new_page(1);
		map(_as, vaddr, page_frame, 1);
		load_helper(page_frame, fd, current, pgsize);
		vaddr += pgsize;
		current += pgsize;
	}
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

